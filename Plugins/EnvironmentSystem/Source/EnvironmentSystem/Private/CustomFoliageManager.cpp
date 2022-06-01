// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFoliageManager.h"

#include "CustomFoliageBase.h"
#include "CustomFoliageISMC.h"
#include "EnvironmentSystem.h"
#include "FoliageSwapper.h"
#include "InstancedFoliageActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACustomFoliageManager::ACustomFoliageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
}

// Called when the game starts or when spawned
void ACustomFoliageManager::BeginPlay()
{
	Super::BeginPlay();
	GetCustomFoliageISMCs();
	StartFoliageSwapLoop();
	
}


void ACustomFoliageManager::CheckInComponent(UFoliageSwapper* ComponentToTrack)
{
	if(ComponentToTrack == nullptr)
	{
		UE_LOG(LogEnvironmentSystem,Error,TEXT("Custom Foliage Manager received null actor to tract"))
		return;
	}

	if(TrackedComponents.Contains(ComponentToTrack))
	{
		UE_LOG(LogEnvironmentSystem,Warning,TEXT("Custom Foliage Manager attempted to track duplicate actor %s"),
			*ComponentToTrack->GetOwner()->GetName())
		return;
	}
	
	TrackedComponents.Add(ComponentToTrack);
	UE_LOG(LogEnvironmentSystem,Log,TEXT("Foliage Manager added %s to tracking"),
		*ComponentToTrack->GetOwner()->GetName())
}

void ACustomFoliageManager::CheckOutComponent(UFoliageSwapper* ComponentToRemove)
{

	if(ComponentToRemove == nullptr)
	{
		UE_LOG(LogEnvironmentSystem,Error,TEXT("Custom Foliage Manager received null actor to remove"))
		return;
	}
	
	if(TrackedComponents.Contains(ComponentToRemove))
	{
		TrackedComponents.Remove(ComponentToRemove);
		UE_LOG(LogEnvironmentSystem,Log,TEXT("Foliage Manager removed %s from tracking"),
			*ComponentToRemove->GetOwner()->GetName())
	}
	else
	{
		UE_LOG(LogEnvironmentSystem,Warning,TEXT("Custom Foliage Manager could not remove %s from tracked components"),
			*ComponentToRemove->GetOwner()->GetName())
	}
}

void ACustomFoliageManager::GetCustomFoliageISMCs()
{
	//Get Instanced Foliage Actor
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), FoundActors);
	
	for (int i = 0; i < FoundActors.Num(); ++i)
	{
		if(const AInstancedFoliageActor* TargetFoliageActor = Cast<AInstancedFoliageActor>(FoundActors[i]))
		{
			TArray<UCustomFoliageISMC*> FoundCustomFoliageISMCs;
			TargetFoliageActor->GetComponents<UCustomFoliageISMC>(FoundCustomFoliageISMCs);
			CustomFoliageISMCs += FoundCustomFoliageISMCs;
		}
	}

	UE_LOG(LogEnvironmentSystem,Warning,TEXT("%s found %d custom foliage ISMCs"),
		*GetName(),CustomFoliageISMCs.Num())
	
}

void ACustomFoliageManager::StartFoliageSwapLoop()
{
	GetWorld()->GetTimerManager().SetTimer(FoliageSwapTimerHandle,this,&ACustomFoliageManager::FoliageSwapLoop,SwapLoopRate,true);
}

void ACustomFoliageManager::FoliageSwapLoop()
{
	SwapInstancesInRangeWithActors();
	SwapActorsOutRangeWithInstances();
}

void ACustomFoliageManager::SwapInstancesInRangeWithActors()
{

	if(HasAuthority() == false){return;}
	
	//Loop through foliage ISMCs
	for (int ISMC = 0; ISMC < CustomFoliageISMCs.Num(); ++ISMC)
	{
		UCustomFoliageISMC* TargetISMC = CustomFoliageISMCs[ISMC];
		
		//Loop through tracked Components to get locations
		TArray<FVector> Locations;
		for (int Swapper = 0; Swapper < TrackedComponents.Num(); ++Swapper)
		{
			Locations.Add(TrackedComponents[Swapper]->GetOwner()->GetActorLocation());
		}

		//Get Instances to Remove
		TArray<int32> IndexesToRemove;
		TargetISMC->GetInstancesInRangeOfLocations(Locations,SwapRange,IndexesToRemove);

		//Get Transforms of Instances
		TArray<FTransform> Transforms;
		for (int i = 0; i < IndexesToRemove.Num(); ++i)
		{
			FTransform TargetTransform;
			if(TargetISMC->GetInstanceTransform(IndexesToRemove[i],TargetTransform,true))
			{
				Transforms.Add(TargetTransform);
			}
			else
			{
				UE_LOG(LogEnvironmentSystem,Error,TEXT("Could not get transform of instance %s of %s"),
					i,*TargetISMC->GetName())
			}
		}
		
		//Spawn Actors
		for (int t = 0; t < Transforms.Num(); ++t)
		{
			//Only Spawn foliage on the Server
			SpawnFoliageActor(Transforms[t],TargetISMC);
		}
	}
}

void ACustomFoliageManager::SwapActorsOutRangeWithInstances()
{

	if(HasAuthority() == false){return;}
	
	//Loop through tracked foliage
	for (int i = TrackedFoliage.Num() - 1; i >= 0; --i)
	{
		//Make sure target foliage is valid
		if(ACustomFoliageBase* TargetFoliage = TrackedFoliage[i])
		{
			TArray<bool> RangeChecks;
			for (int y = 0; y < TrackedComponents.Num(); ++y)
			{
				const UFoliageSwapper* TargetSwapper = TrackedComponents[y];
				const float Distance = FVector::Dist(TargetFoliage->GetActorLocation(),TargetSwapper->GetOwner()->GetActorLocation());

				//Check if tracked component is in range of foliage
				bool bIsInRange = Distance < SwapRange;
				RangeChecks.Add(bIsInRange);
			}

			//Check if any actors are still in range
			if(RangeChecks.Contains(true) == false)
			{
				//Remove destroy and remove from tracking
				TargetFoliage->RequestRemoval();
				TrackedFoliage.RemoveAt(i);
			}
		}
		else
		{
			//If tracked actor null, remove from tracking
			TrackedFoliage.RemoveAt(i);
		}
	}
}

//Only Spawn Actor if Authority
void ACustomFoliageManager::SpawnFoliageActor(const FTransform Transform, UCustomFoliageISMC* FoliageISMC)
{
	if(HasAuthority() == false)
	{
		return;
	}

	if(ACustomFoliageBase* NewFoliage = GetWorld()->SpawnActor<ACustomFoliageBase>(FoliageISMC->FoliageActorClass,Transform))
	{
		TrackedFoliage.Add(NewFoliage);
		NewFoliage->OnSpawned(FoliageISMC,this);
	}
}


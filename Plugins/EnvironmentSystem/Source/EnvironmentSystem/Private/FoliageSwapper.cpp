// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageSwapper.h"

#include "CustomFoliageBase.h"
#include "CustomFoliageISMC.h"
#include "EnvironmentSystem.h"
#include "InstancedFoliageActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFoliageSwapper::UFoliageSwapper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFoliageSwapper::BeginPlay()
{
	Super::BeginPlay();

	GetCustomFoliageISMCs();

	// ...
	
}


// Called every frame
void UFoliageSwapper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SwapInstancesInRange();
	ReplaceSpawnedActors();

	// ...
}

void UFoliageSwapper::GetCustomFoliageISMCs()
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
		*GetOwner()->GetName(),CustomFoliageISMCs.Num())

	if(CustomFoliageISMCs.Num()>0)
	{
		bSwapFoliage = true;
	}
}

void UFoliageSwapper::SwapInstancesInRange()
{

	if(bSwapFoliage==false){return;}

	//Loop through FoliageISMCs
	for (int i = 0; i < CustomFoliageISMCs.Num(); ++i)
	{
		UCustomFoliageISMC* TargetFoliageISMC = CustomFoliageISMCs[i];
		
		//Get Foliage Indexes to remove
		TArray<int32> Instances;
		TargetFoliageISMC->GetInstancesInRange(GetOwner()->GetActorLocation(),SwapDistance,Instances);

		//Check if there are instances in range
		if(Instances.Num() > 0)
		{
			//Get Foliage Transforms
			TArray<FTransform> Transforms;
			for (int t = 0; t < Instances.Num(); ++t)
			{
				FTransform InstanceTransform;
				if(TargetFoliageISMC->GetInstanceTransform(Instances[t],InstanceTransform,true))
				{
					Transforms.Add(InstanceTransform);
				}
			}

			//Remove Indexes
			TargetFoliageISMC->RemoveInstances(Instances);

			//Spawn Actors
			for (int a = 0; a < Transforms.Num(); ++a)
			{
				if (ACustomFoliageBase* NewActor = GetWorld()->SpawnActor<ACustomFoliageBase>(
					TargetFoliageISMC->FoliageActorClass, Transforms[a]))
				{
					NewActor->SetReferences(TargetFoliageISMC);
					SpawnedFoliageActors.Add(NewActor);
				}
			}
		}
	}
}

void UFoliageSwapper::ReplaceSpawnedActors()
{
	for (int i = SpawnedFoliageActors.Num() - 1; i >= 0; --i)
	{
		if(ACustomFoliageBase* TargetFoliageActor = SpawnedFoliageActors[i])
		{
			const float Distance = FVector::Dist(TargetFoliageActor->GetActorLocation(),GetOwner()->GetActorLocation());

			//Check if distance is greater than target distance
			if(Distance > SwapDistance)
			{
				FTransform Transform = TargetFoliageActor->GetActorTransform();
				if(UCustomFoliageISMC* FoliageISMC = TargetFoliageActor->GetOriginatingFoliageISMC())
				{
					//Add Instance and Destroy Actor
					FoliageISMC->AddInstance(Transform,true);
					TargetFoliageActor->Destroy();

					//Remove Index
					SpawnedFoliageActors.RemoveAt(i);
				}
			}
		}
	}
}


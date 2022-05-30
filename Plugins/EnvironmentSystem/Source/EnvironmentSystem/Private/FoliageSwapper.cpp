// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageSwapper.h"
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
}

void UFoliageSwapper::SwapInstancesInRange()
{
	for (int i = 0; i < CustomFoliageISMCs.Num(); ++i)
	{
		UCustomFoliageISMC* TargetFoliageISMC = CustomFoliageISMCs[i];
		
		
		TArray<int32> Instances = TargetFoliageISMC->GetInstancesOverlappingSphere(
			GetOwner()->GetActorLocation(), SwapDistance, true);

		
		
	}
}


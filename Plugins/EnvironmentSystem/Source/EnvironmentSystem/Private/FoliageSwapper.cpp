// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageSwapper.h"

#include "CustomFoliageBase.h"
#include "CustomFoliageManager.h"
#include "EnvironmentSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFoliageSwapper::UFoliageSwapper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UFoliageSwapper::BeginPlay()
{
	Super::BeginPlay();

	CheckInToFoliageManager();
	
}


void UFoliageSwapper::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	CheckOutOfFoliageManager();
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UFoliageSwapper::CheckInToFoliageManager()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomFoliageManager::StaticClass(), FoundActors);

	//If none or more than 1, then log error
	if(FoundActors.Num() == 0 || FoundActors.Num() > 1)
	{
		UE_LOG(LogEnvironmentSystem,Error,TEXT("Found %d Foliage Managers. Expected 1"),FoundActors.Num())
		return;
	}

	FoliageManager = Cast<ACustomFoliageManager>(FoundActors[0]);
	FoliageManager->CheckInComponent(this);
}

void UFoliageSwapper::CheckOutOfFoliageManager()
{
	if(FoliageManager)
	{
		FoliageManager->CheckOutComponent(this);
	}
	
}

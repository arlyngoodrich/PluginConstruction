// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPieceSpawner.h"

#include "BuildingSystem.h"
#include "CustomPlayerController.h"
#include "PlayerInteractionSensor.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UBuildingPieceSpawner::UBuildingPieceSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);

	// ...
}

void UBuildingPieceSpawner::RequestBuildingSpawn(TSubclassOf<ABuildingPiece> PieceToSpawn)
{
	
}


// Called when the game starts
void UBuildingPieceSpawner::BeginPlay()
{
	Super::BeginPlay();

	SetReferences();

	// ...
	
}



void UBuildingPieceSpawner::SetReferences()
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if(OwningPawn == nullptr)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("Building System Component not owned by pawn"))
		return;
	}

	ACustomPlayerController* Controller = Cast<ACustomPlayerController>(OwningPawn);
	if(Controller == nullptr)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("Building System Component pawn controller not Custom Player Controller"))
		return;
	}

	CustomPlayerController = Controller;

	UPlayerInteractionSensor* InteractionSensor = OwningPawn->FindComponentByClass<UPlayerInteractionSensor>();
	if(InteractionSensor == nullptr)
	{UE_LOG(LogBuildingSystem,Error,TEXT("Building System Component pawn does not have player interaction system component"))}

	PlayerInteractionSensor = InteractionSensor;
	bReferencesSet = true;

}

void UBuildingPieceSpawner::StartSpawnLoop()
{
}

void UBuildingPieceSpawner::SpawnLoop()
{
}


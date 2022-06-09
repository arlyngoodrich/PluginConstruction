// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingPiece.h"
#include "BuildingSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABuilding, MyBuildingPieces);

}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABuilding::CheckBuildingPieceIn(ABuildingPiece* BuildingPiece)
{
	if(HasAuthority() == false){return;}
	
	if(BuildingPiece == nullptr)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("%s attempted to check in null building piece"),*GetName())
		return;
	}
	
	if(MyBuildingPieces.Contains(BuildingPiece))
	{
		UE_LOG(LogBuildingSystem,Warning,TEXT("%s attempting to add %s which is already checked in"),
			*GetName(),*BuildingPiece->GetName())
		return;
	}

	UE_LOG(LogBuildingSystem,Log,TEXT("%s checked into %s building"),*BuildingPiece->GetName(),*GetName())
	MyBuildingPieces.Add(BuildingPiece);	
	
}

void ABuilding::CheckBuildingPieceOut(ABuildingPiece* BuildingPiece)
{
	if(BuildingPiece == nullptr)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("%s attempted to checkout null building piece"),*GetName())
		return;
	}

	if(MyBuildingPieces.Contains(BuildingPiece) == false)
	{
		UE_LOG(LogBuildingSystem,Warning,TEXT("%s attempting to check out %s which is not checked in"),
		*GetName(),*BuildingPiece->GetName())
		return;
		
	}

	UE_LOG(LogBuildingSystem,Log,TEXT("%s checked out of %s building"),*BuildingPiece->GetName(),*GetName())
	MyBuildingPieces.Remove(BuildingPiece);	
		
}

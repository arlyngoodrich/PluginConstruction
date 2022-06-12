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

FDateTime ABuilding::GetTimeCreated() const { return TimeCreated;}
TArray<ABuildingPiece*> ABuilding::GetBuildingPieces(){return MyBuildingPieces;}

void ABuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABuilding, MyBuildingPieces);
	DOREPLIFETIME(ABuilding, TimeCreated);
	DOREPLIFETIME(ABuilding, RootPiece);
	
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	//Set time created 
	if(HasAuthority())
	{
		TimeCreated = FDateTime::UtcNow();
		if(bSpawnedAsTemplateBuilding)
		{
			InitializeFromTemplate();
		}
	}
	
}

void ABuilding::InitializeFromTemplate()
{
	if(HasAuthority() == false){return;}

	//Get Building Pieces form Child Actors
	TArray<AActor*> ChildActors;
	GetAllChildActors(ChildActors,true);
	for (int i = 0; i < ChildActors.Num(); ++i)
	{
		AActor* TargetActor = ChildActors[i];
		if(ABuildingPiece* TargetBuildingPiece = Cast<ABuildingPiece>(TargetActor))
		{
			//Set owning Building and check in
			TargetBuildingPiece->SetOwningBuilding(this);
			CheckBuildingPieceIn(TargetBuildingPiece);
		}
	}

	for (int i = 0; i < MyBuildingPieces.Num(); ++i)
	{
		//Have owned building pieces update their support points
		if(MyBuildingPieces[i])
		{
			MyBuildingPieces[i]->UpdateSupportPoints();
		}
	}

	UE_LOG(LogBuildingSystem,Log,TEXT("%s spawned from template"),*GetName())
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

void ABuilding::MergeBuilding(ABuilding* TargetBuilding)
{
	if(HasAuthority() == false){return;}
	
	TArray<ABuildingPiece*> BuildingPiecesToAdd = TargetBuilding->GetBuildingPieces();

	for (int i = 0; i < BuildingPiecesToAdd.Num(); ++i)
	{
		if(ABuildingPiece* TargetPiece = BuildingPiecesToAdd[i])
		{
			TargetPiece->SetOwningBuilding(this);
			CheckBuildingPieceIn(TargetPiece);
		}
	}

	UE_LOG(LogBuildingSystem,Log,TEXT("%s merged into %s"),*TargetBuilding->GetName(),*GetName())
	TargetBuilding->Destroy();
}

void ABuilding::SetRootPiece(ABuildingPiece* NewRootPiece)
{

	if(NewRootPiece == nullptr){return;}
	
	RootPiece = NewRootPiece;
	UE_LOG(LogBuildingSystem,Log,TEXT("%s set as root for %s building"),*NewRootPiece->GetName(),*GetName())
}

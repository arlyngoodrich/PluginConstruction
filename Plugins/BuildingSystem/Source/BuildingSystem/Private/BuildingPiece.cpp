// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPiece.h"

#include "Building.h"
#include "BuildingPieceSnapPoint.h"
#include "BuildingSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABuildingPiece::ABuildingPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

void ABuildingPiece::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABuildingPiece, bIsSnapped);
	DOREPLIFETIME(ABuildingPiece, CurrentInstability);

	
}

bool ABuildingPiece::GetShouldCheckForSnaps() const { return bCheckForSnaps; }
ABuilding* ABuildingPiece::GetOwningBuilding() const {return OwningBuilding;}

void ABuildingPiece::SetOwningBuilding(ABuilding* NewOwningBuilding)
{
	if(HasAuthority() == false ){return;}

	if(NewOwningBuilding == nullptr){return;}

	OwningBuilding = NewOwningBuilding;
}

void ABuildingPiece::OnSpawnStart()
{
	if(HasAuthority())
	{
		SetReplicates(false);
		if(GetLocalRole()==ROLE_AutonomousProxy)
		{
			Destroy();
		}
	}

}

// Called when the game starts or when spawned
void ABuildingPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildingPiece::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(EndPlayReason == EEndPlayReason::Destroyed && OwningBuilding && HasAuthority())
	{
		OwningBuilding->CheckBuildingPieceOut(this);
	}
	
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void ABuildingPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABuildingPiece::OnPlaced()
{
	if(HasAuthority() == false){return;}


	//Get Connecting Building Pieces
	UpdateSupportPoints();

	//Check overlapping pieces for unique buildings
	TArray<ABuilding*> FoundBuildings;
	for (int i = 0; i < SupportingBuildingPieces.Num(); ++i)
	{
		const ABuildingPiece* TargetPiece = SupportingBuildingPieces[i];
		ABuilding* TargetBuilding = TargetPiece->GetOwningBuilding();
		if(TargetBuilding != nullptr && FoundBuildings.Contains(TargetBuilding) == false)
		{
			FoundBuildings.Add(TargetBuilding);
		}
	}

	//If no found buildings, spawn a new one and make this the root piece
	if(FoundBuildings.Num() == 0)
	{
		const FActorSpawnParameters SpawnParameters;
		if(ABuilding* NewBuilding = GetWorld()->SpawnActor<ABuilding>(ABuilding::StaticClass(), GetTransform(), SpawnParameters))
		{
			NewBuilding->CheckBuildingPieceIn(this);
			OwningBuilding = NewBuilding;
			OwningBuilding->SetRootPiece(this);
			
			UE_LOG(LogBuildingSystem,Log,TEXT("%s created new building %s"),*GetName(),*NewBuilding->GetName())
		}
	}
	//If one building, check in
	else if(FoundBuildings.Num() == 1)
	{
		FoundBuildings[0]->CheckBuildingPieceIn(this);
		OwningBuilding = FoundBuildings[0];
	}
	//If more than one building, merge with older one?
	else
	{
		TArray<FDateTime> DateTimes;
		for (int i = 0; i < FoundBuildings.Num(); ++i)
		{
			FDateTime DateTime = FoundBuildings[i]->GetTimeCreated();
			DateTimes.Add(DateTime);
		}

		//Find oldest building
		int32 OldIndex;
		FMath::Min<FDateTime>(DateTimes,&OldIndex);
		ABuilding* OldestBuilding = FoundBuildings[OldIndex];

		//Find newest building
		int32 NewIndex;
		FMath::Max<FDateTime>(DateTimes,&NewIndex);
		ABuilding* NewestBuilding = FoundBuildings[NewIndex];

		//Check into oldest building and trigger merge
		OldestBuilding->CheckBuildingPieceIn(this);
		OldestBuilding->MergeBuilding(NewestBuilding);
			
	}
}

void ABuildingPiece::UpdateSupportPoints()
{

	
	SupportingBuildingPieces.Empty();
	
	//Check for overlapping snap points
	TArray<UMeshComponent*> MeshComponents;
	TArray<UBuildingPieceSnapPoint*> MyOwnedSnapPoints;

	GetComponents<UBuildingPieceSnapPoint>(MyOwnedSnapPoints);
	GetComponents<UMeshComponent>(MeshComponents);

	TArray<bool> WorldStaticChecks;
	TArray<bool> BuildingPieceChecks;

	//Cycle through all mesh components to find overlapping snapped points 
	for (int i = 0; i < MeshComponents.Num(); ++i)
	{
		const UMeshComponent* TargetMeshComponent = MeshComponents[i];
		TArray<UPrimitiveComponent*> PrimitiveComponents;
		TargetMeshComponent->GetOverlappingComponents(PrimitiveComponents);

		//Cycle through overlapped primitives looking for overlapping building pieces 
		for (int c = 0; c < PrimitiveComponents.Num(); ++c)
		{
			const UPrimitiveComponent* TargetPrimitiveComponent = PrimitiveComponents[c];

			//Check if overlapping building piece
			if(ABuildingPiece* OverlappedBuildingPiece = Cast<ABuildingPiece>(TargetPrimitiveComponent->GetOwner()))
			{
				SupportingBuildingPieces.Add(OverlappedBuildingPiece);
			}
			else
			{
				//If not check, if world static
				bool bIsWorldStatic = TargetPrimitiveComponent->GetCollisionObjectType()==ECC_WorldStatic;
				WorldStaticChecks.Add(bIsWorldStatic);
			}
		}
	}

	bIsOverlappingBuildingPiece = SupportingBuildingPieces.Num() > 0;
	bIsOverlappingWorldStatic = WorldStaticChecks.Contains(true);
}



//BP Version
bool ABuildingPiece::CheckPlacement_Implementation(const bool bIsSnappedDuringSpawn)
{
	
	return Internal_CheckPlacement(bIsSnappedDuringSpawn);
}


bool ABuildingPiece::Internal_CheckPlacement(const bool bIsSnappedDuringSpawn)
{
	//Check if overlapping world static or another building piece
	UpdateSupportPoints();
	
	return bIsOverlappingBuildingPiece || bIsOverlappingWorldStatic || bIsSnappedDuringSpawn;
}



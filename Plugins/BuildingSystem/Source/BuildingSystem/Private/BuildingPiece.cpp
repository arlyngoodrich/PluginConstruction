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

int32 ABuildingPiece::GetCurrentInstability() { return CurrentInstability;}

ABuilding* ABuildingPiece::GetOwningBuilding() const {return OwningBuilding;}

void ABuildingPiece::SetOwningBuilding(ABuilding* NewOwningBuilding)
{
	if(HasAuthority() == false ){return;}

	if(NewOwningBuilding == nullptr){return;}

	OwningBuilding = NewOwningBuilding;
}

void ABuildingPiece::RemoveBuildingPiece()
{
	if(HasAuthority()==false)
	{
		Server_RemoveBuildingPiece();
		return;
	}

	//Tell Building to remove this piece
	if(OwningBuilding == nullptr)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("%s does not have a valid owning building"),*GetName())
		return;
	}

	OwningBuilding->RemoveBuildingPiece(this);
	
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

	CalculateInstability();

}


void ABuildingPiece::UpdateSupportPoints()
{
	
	SupportingBuildingPieces.Empty();
	
	//Check for overlapping primitive components
	TArray<UMeshComponent*> MeshComponents;
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
				//Make sure it's not this piece
				if(OverlappedBuildingPiece !=this)
				{
					SupportingBuildingPieces.Add(OverlappedBuildingPiece);
				}
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


void ABuildingPiece::CalculateInstability()
{

	if(bIsOverlappingWorldStatic)
	{
		CurrentInstability = 0;
	}
	else if(bIsOverlappingBuildingPiece)
	{
		TArray<int32> ConnectedInstability;
		for (int i = 0; i < SupportingBuildingPieces.Num(); ++i)
		{
			ConnectedInstability.Add(SupportingBuildingPieces[i]->GetCurrentInstability());
		}
		
		if(ConnectedInstability.Num()>0)
		{
			int32 MinIndex;
			FMath::Min(ConnectedInstability,&MinIndex);
			const int32 MinInstability = ConnectedInstability[MinIndex];
			CurrentInstability = MinInstability + 1;
		}
		else
		{
			UE_LOG(LogBuildingSystem,Warning,TEXT("%s could not find the stabilty for connected pieces"),*GetName())
		}

	}
	else
	{
		UE_LOG(LogBuildingSystem,Warning,TEXT("%s is not overlapping world static or building piece"),*GetName())
	}

	UE_LOG(LogBuildingSystem,Log,TEXT("%s instabiltiy = %d"),*GetName(),CurrentInstability);
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



bool ABuildingPiece::Server_RemoveBuildingPiece_Validate()
{
	return true;
}

void ABuildingPiece::Server_RemoveBuildingPiece_Implementation()
{
	RemoveBuildingPiece();
}


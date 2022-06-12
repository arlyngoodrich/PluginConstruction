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
	DOREPLIFETIME(ABuildingPiece, SupportingSnapPoints);
	DOREPLIFETIME(ABuildingPiece, CurrentInstability);

	
}

bool ABuildingPiece::GetShouldCheckForSnaps() const { return bCheckForSnaps; }
ABuilding* ABuildingPiece::GetOwningBuilding(){return OwningBuilding;}

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


void ABuildingPiece::OnPlaced(const bool SetIsSnapped)
{

	if(HasAuthority() == false){return;}
	
	bIsSnapped = SetIsSnapped;
	if(bIsSnapped)
	{
		UpdateSupportPoints();

		//Make sure there are valid snap points
		if(SupportingSnapPoints.Num()==0)
		{
			UE_LOG(LogBuildingSystem,Error,TEXT("%s marked as snapped but could not find any valid snap points"),
				*GetName())
			return;
		}

		TArray<ABuilding*> FoundBuildings;
		//Check for owning building --> error if not
		for (int i = 0; i < SupportingSnapPoints.Num(); ++i)
		{
			const UBuildingPieceSnapPoint* TargetSnapPoint = SupportingSnapPoints[i];
			ABuildingPiece* TargetPiece = TargetSnapPoint->GetOwningPiece();
			ABuilding* TargetBuilding = TargetPiece->GetOwningBuilding();
			if(TargetBuilding != nullptr && FoundBuildings.Contains(TargetBuilding) == false)
			{
				FoundBuildings.Add(TargetBuilding);
			}
		}

		//If no found buildings, error
		if(FoundBuildings.Num() == 0)
		{
			UE_LOG(LogBuildingSystem,Error,TEXT("%s attempted to check into building but not building was found"),
				*GetName())
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
	else
	{
		//If no snap points/owning building, create one
		const FActorSpawnParameters SpawnParameters;
		if(ABuilding* NewBuilding = GetWorld()->SpawnActor<ABuilding>(ABuilding::StaticClass(), GetTransform(), SpawnParameters))
		{
			NewBuilding->CheckBuildingPieceIn(this);
			OwningBuilding = NewBuilding;
			UE_LOG(LogBuildingSystem,Log,TEXT("%s created new building %s"),*GetName(),*NewBuilding->GetName())
		}
	}
}

void ABuildingPiece::UpdateSupportPoints()
{

	SupportingSnapPoints.Empty();
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

		//Cycle through overlapped primitives looking for snap point components
		for (int c = 0; c < PrimitiveComponents.Num(); ++c)
		{
			UPrimitiveComponent* TargetPrimitiveComponent = PrimitiveComponents[c];

			//Check if overlapping building piece
			if(ABuildingPiece* OverlappedBuildingPiece = Cast<ABuildingPiece>(TargetPrimitiveComponent->GetOwner()))
			{
				SupportingBuildingPieces.Add(OverlappedBuildingPiece);
			}
							
			//Check if snap point
			if(UBuildingPieceSnapPoint* SnapPoint = Cast<UBuildingPieceSnapPoint>(TargetPrimitiveComponent))
			{
				//Make sure snap point is eligible to provide support and that its not one of this building pieces own snap points
				if(SnapPoint->IsEligibleForSupport(this->GetClass()) && MyOwnedSnapPoints.Contains(SnapPoint) == false)
				{
					//Add valid overlapping snap points to snap points array
					SupportingSnapPoints.Add(SnapPoint);
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

void ABuildingPiece::CalculateInstability()
{
	TArray<int32> SupportPiecesInstability;
	for (int i = 0; i < SupportingBuildingPieces.Num(); ++i)
	{
		
	}
}

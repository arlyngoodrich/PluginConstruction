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
}

bool ABuildingPiece::GetShouldCheckForSnaps() const { return bCheckForSnaps; }
ABuilding* ABuildingPiece::GetOwningBuilding(){return OwningBuilding;}

void ABuildingPiece::OnPlacementStart()
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

void ABuildingPiece::BeginDestroy()
{
	if(OwningBuilding && HasAuthority())
	{
		OwningBuilding->CheckBuildingPieceOut(this);
	}
	
	Super::BeginDestroy();
}


// Called every frame
void ABuildingPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingPiece::OnPlaced(const bool SetIsSnapped)
{

	if(HasAuthority() == false){return;}

	SupportingSnapPoints.Empty();

	bIsSnapped = SetIsSnapped;
	if(bIsSnapped)
	{
		//Check for overlapping snap points
		TArray<UMeshComponent*> MeshComponents;
		TArray<UBuildingPieceSnapPoint*> MyOwnedSnapPoints;

		GetComponents<UBuildingPieceSnapPoint>(MyOwnedSnapPoints);
		GetComponents<UMeshComponent>(MeshComponents);

		//Cycyke through all mesh components to find overlapping snapped points 
		for (int i = 0; i < MeshComponents.Num(); ++i)
		{
			const UMeshComponent* TargetMeshComponent = MeshComponents[i];
			TArray<UPrimitiveComponent*> PrimitiveComponents;
			TargetMeshComponent->GetOverlappingComponents(PrimitiveComponents);

			//Cycle through overlapped primitives looking for snap point components
			for (int c = 0; c < PrimitiveComponents.Num(); ++c)
			{
				UPrimitiveComponent* TargetPrimitiveComponent = PrimitiveComponents[c];
				if(UBuildingPieceSnapPoint* SnapPoint = Cast<UBuildingPieceSnapPoint>(TargetPrimitiveComponent))
				{
					//Make sure snap point is eligible to provide support and that its not one of this building pieces own snap points
					if(SnapPoint->IsEligibleForSupport(this->GetClass()) && MyOwnedSnapPoints.Contains(SnapPoint) == false)
					{
						//Add valid overlapping snap points to snap points array
						SupportingSnapPoints.Add(SnapPoint);
					}
				}
			}
		}

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
			UE_LOG(LogBuildingSystem,Warning,TEXT("Multiple Buildings Found, need to implement building merge"))
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
		else
		{
			UE_LOG(LogBuildingSystem,Error,TEXT("%s could not spawn building"),*GetName())
		}
	}
}




bool ABuildingPiece::CheckPlacement_Implementation()
{
	return Internal_CheckPlacement();
}


bool ABuildingPiece::Internal_CheckPlacement()
{
	return true;
}
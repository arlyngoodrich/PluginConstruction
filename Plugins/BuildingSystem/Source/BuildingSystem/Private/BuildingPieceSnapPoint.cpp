// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPieceSnapPoint.h"
#include "BuildingPiece.h"
#include "BuildingSystem.h"
#include "Net/UnrealNetwork.h"

UBuildingPieceSnapPoint::UBuildingPieceSnapPoint()
{
	SetIsReplicated(true);
	SphereRadius = 5;
}


void UBuildingPieceSnapPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildingPieceSnapPoint, bIsSnapped);
	DOREPLIFETIME(UBuildingPieceSnapPoint, SnappedPieces);
}

ABuildingPiece* UBuildingPieceSnapPoint::GetOwningPiece() const {return OwningPiece;}

TArray<TSubclassOf<ABuildingPiece>> UBuildingPieceSnapPoint::GetEligibleBuildingPieces() const {return AcceptableSnapClasses;}

bool UBuildingPieceSnapPoint::IsEligibleForSnap(ABuildingPiece* PieceToCheck)
{
	if(PieceToCheck == nullptr){return false;}
	
	TArray<bool> ClassChecks;
	for (int i = 0; i < AcceptableSnapClasses.Num(); ++i)
	{
		bool ClassCheck = PieceToCheck->GetClass()->IsChildOf(AcceptableSnapClasses[i]) || PieceToCheck->GetClass() == AcceptableSnapClasses[i];
		ClassChecks.Add(ClassCheck);
	}
	
	TArray<bool> RotationChecks;
	for (int i = SnappedPieces.Num() - 1; i >= 0; --i)
	{
		ABuildingPiece* TargetPiece = SnappedPieces[i];
		if(SnappedPieces[i] != nullptr)
		{
			//Check if it's a subclass or the same class as the target snap piece.
			if(PieceToCheck->GetClass()->IsChildOf(TargetPiece->GetClass())||PieceToCheck->GetClass() == TargetPiece->GetClass())
			{
				bool RotationCheck = TargetPiece->GetActorRotation() == PieceToCheck->GetActorRotation();
				RotationChecks.Add(RotationCheck);
			}
			else
			{
				RotationChecks.Add(false);
			}
		}
		else
		{
			SnappedPieces.Remove(TargetPiece);
		}
	}
	
	return ClassChecks.Contains(true) && !RotationChecks.Contains(true);
}


void UBuildingPieceSnapPoint::AddSnappedPiece(ABuildingPiece* Piece)
{
	if(Piece == nullptr){ return;}
	
	SnappedPieces.Add(Piece);
	bIsSnapped = true;

	Piece->OnBeginDestroy.AddDynamic(this,&UBuildingPieceSnapPoint::OnPieceBeginDestroy);
}


void UBuildingPieceSnapPoint::BeginPlay()
{
	Super::BeginPlay();


	OwningPiece = Cast<ABuildingPiece>(GetOwner());
	if(OwningPiece == nullptr)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("Buiding Snap point component owner is not building piece but %s"),
			*GetOwner()->GetName())
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&UBuildingPieceSnapPoint::CheckForDuplicatedSnapPoints,.5,false);
}

void UBuildingPieceSnapPoint::OnPieceBeginDestroy(ABuildingPiece* DestroyedPiece)
{
	
	if(SnappedPieces.Contains(DestroyedPiece))
	{
		SnappedPieces.Remove(DestroyedPiece);
		DestroyedPiece->OnBeginDestroy.RemoveDynamic(this,&UBuildingPieceSnapPoint::OnPieceBeginDestroy);
	}
}


void UBuildingPieceSnapPoint::CheckForDuplicatedSnapPoints()
{
	AcceptableSnapClasses = DefaultAcceptableSnapClasses;
	
	TArray<UPrimitiveComponent*> Components;
	GetOverlappingComponents(Components);

	//Go through all overlapping components
	for (int i = 0; i < Components.Num(); ++i)
	{
		//Check to see if it's a snap point
		if(const UBuildingPieceSnapPoint* TargetSnapPoint = Cast<UBuildingPieceSnapPoint>(Components[i]))
		{
			//If the snap points are in the same location
			if(GetComponentLocation().Equals(TargetSnapPoint->GetComponentLocation(),.1f))
			{
				//Get the eligible building classes
				auto TargetSnapPointClasses = TargetSnapPoint->GetEligibleBuildingPieces();
				UE_LOG(LogBuildingSystem,Log,TEXT("Overlapped snap point has %d acceptable classes"),TargetSnapPointClasses.Num())

				for (int t = 0; t < TargetSnapPointClasses.Num(); ++t)
				{
					//If it has an eligible building class that this snap point has
					if(AcceptableSnapClasses.Contains(TargetSnapPointClasses[t]))
					{
						//remove the eligible building class form this snap point
						AcceptableSnapClasses.Remove(TargetSnapPointClasses[t]);
						UE_LOG(LogBuildingSystem,Log,TEXT("Removed %s class from acceptable classes"),*TargetSnapPointClasses[t]->GetName())
					}
				}
			}
			
		}
	}
}


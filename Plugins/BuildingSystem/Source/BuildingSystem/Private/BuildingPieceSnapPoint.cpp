// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPieceSnapPoint.h"
#include "BuildingPiece.h"
#include "BuildingSystem.h"
#include "Net/UnrealNetwork.h"

UBuildingPieceSnapPoint::UBuildingPieceSnapPoint()
{
	SetIsReplicated(true);
}


void UBuildingPieceSnapPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildingPieceSnapPoint, bIsSnapped);
	DOREPLIFETIME(UBuildingPieceSnapPoint, SnappedPiece);
}

ABuildingPiece* UBuildingPieceSnapPoint::GetOwningPiece() const {return OwningPiece;}

bool UBuildingPieceSnapPoint::IsEligibleForSnap(const TSubclassOf<ABuildingPiece> Class)
{

	if(bIsSnapped){return false;}
	
	TArray<bool> ClassChecks;
	
	for (int i = 0; i < AcceptableSnapClasses.Num(); ++i)
	{
		bool ClassCheck = Class->IsChildOf(AcceptableSnapClasses[i]);
		ClassChecks.Add(ClassCheck);
	}

	return ClassChecks.Contains(true);
}

bool UBuildingPieceSnapPoint::IsEligibleForSupport(const TSubclassOf<ABuildingPiece> Class)
{
	TArray<bool> ClassChecks;
	
	for (int i = 0; i < AcceptableSnapClasses.Num(); ++i)
	{
		bool ClassCheck = Class->IsChildOf(AcceptableSnapClasses[i]);
		ClassChecks.Add(ClassCheck);
	}

	return ClassChecks.Contains(true);
}

void UBuildingPieceSnapPoint::AddSnappedPiece(ABuildingPiece* Piece)
{
	SnappedPiece = Piece;
	bIsSnapped = true;
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
	
}

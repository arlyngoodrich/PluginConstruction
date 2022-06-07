// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPieceSnapPoint.h"
#include "BuildingPiece.h"
#include "Net/UnrealNetwork.h"

UBuildingPieceSnapPoint::UBuildingPieceSnapPoint()
{
	SetIsReplicated(true);
}

TArray<TSubclassOf<ABuildingPiece>> UBuildingPieceSnapPoint::GetAcceptableSnapClasses() {return AcceptableSnapClasses;}

void UBuildingPieceSnapPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildingPieceSnapPoint, bIsSnapped);
	DOREPLIFETIME(UBuildingPieceSnapPoint, SnappedPiece);
}

bool UBuildingPieceSnapPoint::IsEligibleClass(TSubclassOf<ABuildingPiece> Class)
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

void UBuildingPieceSnapPoint::AddSnappedPiece(ABuildingPiece* Piece)
{
	SnappedPiece = Piece;
	bIsSnapped = true;
}

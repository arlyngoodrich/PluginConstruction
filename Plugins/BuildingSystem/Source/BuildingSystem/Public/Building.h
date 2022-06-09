// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

class ABuildingPiece;

UCLASS()
class BUILDINGSYSTEM_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	/**
	 * @brief Adds building piece to MyBuildingPieces array.  Ensures not null pointer and will not add duplicate.
	 * @param BuildingPiece Building Piece to add
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void CheckBuildingPieceIn(ABuildingPiece* BuildingPiece);

	/**
	 * @brief Removes Building Piece from MyBuildingPieces array.  Ensures not null pointer. 
	 * @param BuildingPiece Building Piece to remove
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void CheckBuildingPieceOut(ABuildingPiece* BuildingPiece);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Building pieces that make up this building
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Building System")
	TArray<ABuildingPiece*> MyBuildingPieces;

	//TODO add time created 


};

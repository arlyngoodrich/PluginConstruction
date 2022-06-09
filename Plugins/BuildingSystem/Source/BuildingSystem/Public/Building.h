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
	 * @brief Get when the building was created
	 * @return UTC Date time building was created
	 */
	UFUNCTION(BlueprintCallable,Category="Building System")
	FDateTime GetTimeCreated() const;

	/**
	 * @return All owned building pieces
	 */
	UFUNCTION(BlueprintCallable,Category="Building System")
	TArray<ABuildingPiece*> GetBuildingPieces();

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

	/**
	 * @brief Merges the target building pieces into the instigating (this) building piece
	 * @param TargetBuilding Building to Merge into this building
	 */
	void MergeBuilding(ABuilding* TargetBuilding);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Building pieces that make up this building
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Building System")
	TArray<ABuildingPiece*> MyBuildingPieces;

	/**
	 * @brief Set on being play. Set with UTC.
	 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building System")
	FDateTime TimeCreated;


};

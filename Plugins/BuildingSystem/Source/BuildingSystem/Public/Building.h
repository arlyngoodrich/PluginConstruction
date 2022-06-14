// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

class ABuildingPiece;

UCLASS(ClassGroup=(BulidingSystem), Blueprintable)
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
	 * @brief Merges the target building pieces into the instigating (this) building piece
	 * @param TargetBuilding Building to Merge into this building
	 */
	void MergeBuilding(ABuilding* TargetBuilding);

	/**
	 * @brief Sets a new root piece for the building.  Should be done when a new building is made.
	 */
	void SetRootPiece(ABuildingPiece* NewRootPiece);

	/**
	 * @brief Called by the individual building piece to be removed.  Building then destroys the piece and has the other
	 * pieces in the building update their instability.  If this is the last piece, then the building destroys itself too  
	 * @param BuildingPiece pointer to building piece that should be destroyed
	 * @param bDoStabilityCheck true if removing individual piece, false if removing multiple.
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void RemoveBuildingPiece(ABuildingPiece* BuildingPiece,bool bDoStabilityCheck);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief If the building is placed in the world with building piece child actors
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Building System")
	bool bSpawnedAsTemplateBuilding = false;

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

	/**
	 * @brief The root building piece
	 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building System")
	ABuildingPiece* RootPiece = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="Buiding System")
	FGuid StabilityUpdateGUID;

	/**
	 * @brief Called if using a building blueprint in editor
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void InitializeFromTemplate();

	
	/**
	 * @brief Removes Building Piece from MyBuildingPieces array.  Ensures not null pointer. 
	 * @param BuildingPiece Building Piece to remove
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void CheckBuildingPieceOut(ABuildingPiece* BuildingPiece);

	/**
	 * @brief Reverse loop through building pieces to remove unstable pieces 
	 */
	UFUNCTION()
	void RemoveUnstablePieces();

	UFUNCTION()
	void DoStabilityCheck();
	
	UFUNCTION()
	void CheckStabilityUpdateGUIDs();


};

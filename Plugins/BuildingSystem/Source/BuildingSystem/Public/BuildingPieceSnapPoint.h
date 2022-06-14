// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BuildingPieceSnapPoint.generated.h"

class ABuildingPiece;

/**
 * 
 */
UCLASS(ClassGroup=(BulidingSystem), Blueprintable, meta=(BlueprintSpawnableComponent))
class BUILDINGSYSTEM_API UBuildingPieceSnapPoint : public USphereComponent
{
	GENERATED_BODY()

public:

	UBuildingPieceSnapPoint();

	/**
	 * @brief Gets the snap point owning building piece
	 */
	UFUNCTION(BlueprintCallable,Category="Building System")
	ABuildingPiece* GetOwningPiece() const;

	/**
	* @brief Gets the building piece classes that are eligible to snap tp this point
	*/
	UFUNCTION(BlueprintCallable,Category="Building System")
	TArray<TSubclassOf<ABuildingPiece>>  GetEligibleBuildingPieces() const;
	
	/**
	 * @brief Checks if a building piece is eligible to snap to the snap point
	 * @param PieceToCheck Piece to check if can snap
	 * @return True if OK to snap, false if not
	 */
	UFUNCTION(BlueprintCallable,Category="Building System")
	bool IsEligibleForSnap(ABuildingPiece* PieceToCheck);

	/**
	 * @brief Sets the building piece as the snapped piece
	 * @param Piece Piece to snap
	 */
	void AddSnappedPiece(ABuildingPiece* Piece);

	/**
	 * @brief Checks for snap points that are overlapping, if any overlapping snap points have the same snap classes,
	 * remove that snap class from this snap point.
	 */
	void CheckForDuplicatedSnapPoints();
	

protected:

	/**
	 * @brief Building piece classes that can snap to this component
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Building System")
	TArray<TSubclassOf<ABuildingPiece>> DefaultAcceptableSnapClasses;

	/**
	* @brief Building piece classes that can snap to this component
	*/
	UPROPERTY(BlueprintReadOnly,Category="Building System")
	TArray<TSubclassOf<ABuildingPiece>> AcceptableSnapClasses;

	/**
	 * @brief boolean for if snap point already has a piece snapped to it
	 */
	UPROPERTY(Replicated, BlueprintReadOnly,Category="Building System")
	bool bIsSnapped;

	/**
	 * @brief Piece that this snap point is a component of
	 */
	UPROPERTY(Replicated,BlueprintReadOnly,Category="Buidling System")
	ABuildingPiece* OwningPiece;
	
	/**
	 * @brief Piece that this connected to this snap point
	 */
	UPROPERTY(Replicated, BlueprintReadOnly,Category="Building System")
	TArray<ABuildingPiece*> SnappedPieces;
	
	virtual void BeginPlay() override;

		
};

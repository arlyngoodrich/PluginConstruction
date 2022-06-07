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
	 * @brief Building Piece classes that can snap to this component
	 */
	UFUNCTION(BlueprintCallable,Category="Building System")
	TArray<TSubclassOf<ABuildingPiece>> GetAcceptableSnapClasses();

	UFUNCTION(BlueprintCallable,Category="Building System")
	bool IsEligibleClass(TSubclassOf<ABuildingPiece> Class);

	void AddSnappedPiece(ABuildingPiece* Piece);

protected:

	/**
	 * @brief Building piece classes that can snap to this component
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Building System")
	TArray<TSubclassOf<ABuildingPiece>> AcceptableSnapClasses;

	UPROPERTY(Replicated, BlueprintReadOnly,Category="Building System")
	bool bIsSnapped;

	UPROPERTY(Replicated, BlueprintReadOnly,Category="Building System")
	ABuildingPiece* SnappedPiece;
		
};

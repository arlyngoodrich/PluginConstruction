// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingPiece.generated.h"

class UBuildingPieceSnapPoint;
class ABuilding;

UCLASS()
class BUILDINGSYSTEM_API ABuildingPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingPiece();

	UFUNCTION(BlueprintCallable,Category="Building System")
	bool GetShouldCheckForSnaps() const;

	/**
	 * @brief Called by Building Piece Spawner 
	 */
	void OnPlacementStart();

	/**
	 * @brief Blueprint version to check placement.  Defaults to native Internal_CheckPlacement method but can be overriden.
	 * @return True if OK to place, false if not.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Building System")
	bool CheckPlacement();

	/**
	 * @brief Native version of check placement. 
	 * @return True if OK to place, false if not. 
	 */
	virtual bool Internal_CheckPlacement();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void OnPlaced(bool SetIsSnapped);

	UFUNCTION(BlueprintCallable,Category="Building System")
	ABuilding* GetOwningBuilding();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Building System")
	bool bCheckForSnaps = true;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building System")
	bool bIsSnapped;

	UPROPERTY(BlueprintReadOnly,Replicated,Category="Building System")
	TArray<UBuildingPieceSnapPoint*> SupportingSnapPoints;

	UPROPERTY(BlueprintReadOnly,Replicated,Category="Building System")
	ABuilding* OwningBuilding = nullptr;

	

};

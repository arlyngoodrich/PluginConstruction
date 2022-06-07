// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingPiece.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Building System")
	bool bCheckForSnaps = true;
	

};

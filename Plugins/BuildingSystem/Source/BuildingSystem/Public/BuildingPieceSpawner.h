// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingPieceSpawner.generated.h"

class ABuildingPiece;
class UPlayerInteractionSensor;
class ACustomPlayerController;


UCLASS( ClassGroup=(BulidingSystem), meta=(BlueprintSpawnableComponent) )
class BUILDINGSYSTEM_API UBuildingPieceSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildingPieceSpawner();

	UFUNCTION(BlueprintCallable,Category="Building System")
	void RequestBuildingSpawn(TSubclassOf<ABuildingPiece> PieceToSpawn);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * @brief Rate for updating the spawn position of the ghost mesh
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Bulding System")
	float SpawnLoopRate;

	/**
	 * @brief TEMP PROPERTY! 
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Building System")
	TSubclassOf<ABuildingPiece> TestPieceToSpawn;

	/**
	 * @brief Spawn Loop Timer Property
	 */
	UPROPERTY(BlueprintReadOnly,Category="Building System")
	FTimerHandle SpawnLoopTimer;

	/**
	 * @brief Pointer to Player Interaction Sensor.  Used to get player look position. 
	 */
	UPROPERTY(BlueprintReadOnly, Category="Building System")
	UPlayerInteractionSensor* PlayerInteractionSensor;

	/**
	 * @brief Pointer to Universal Core Assets Player controller.  Used for getting input bindings.  
	 */
	UPROPERTY(BlueprintReadOnly,Category="Building System")
	ACustomPlayerController* CustomPlayerController;

	/**
	 * @brief Set true if Interaction Sensor and Custom Player Controller are successfully set in SetReferences method
	 */
	UPROPERTY(BlueprintReadOnly,Category="Building System")
	bool bReferencesSet = false;

	/**
	 * @brief Set pointer references from owning character
	 */
	void SetReferences();

	/**
	 * @brief Spawn ghost mesh and start the spawn loop timer
	 */
	void StartSpawnLoop();

	/**
	 * @brief Update the transform of the ghost mesh until canceled or placed
	 */
	void SpawnLoop();
	
	

		
};

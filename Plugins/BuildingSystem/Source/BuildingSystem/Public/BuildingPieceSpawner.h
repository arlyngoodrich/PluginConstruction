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
	 * @brief Class chosen by player to spawn
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Building System")
	TSubclassOf<ABuildingPiece> BuildingPieceClass;

	/**
	 * @brief Ghost building piece to show player where building piece will spawn.
	 */
	UPROPERTY(BlueprintReadOnly,Category="Building System")
	ABuildingPiece* GhostPiece;

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

	UPROPERTY(BlueprintReadOnly,Category="Building System")
	bool bIsPlacingBuildingPiece;

	/**
	 * @brief Set pointer references from owning character
	 */
	void SetReferences();

	void SetBindings();

	void ClearBindings();

	/**
	 * @brief Spawn ghost mesh and start the spawn loop timer
	 */
	void StartSpawnLoop();

	/**
	 * @brief Spawns Ghost mesh 
	 */
	void SpawnGhostPiece(ABuildingPiece*& OutBuildingPiece) const;

	/**
	 * @brief Update the transform of the ghost mesh until canceled or placed
	 */
	void SpawnLoop();

	/**
	 * @brief Takes Location of Ghost mesh, ends spawn loop, and spawns a new finalized version 
	 */
	void FinalizePlacement();

	/**
	 * @brief Destroys ghost mesh and ends spawn loop
	 */
	void EndSpawnLoop();
		
};

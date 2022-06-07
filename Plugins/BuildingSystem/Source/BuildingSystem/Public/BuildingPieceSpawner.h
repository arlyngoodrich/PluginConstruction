// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingPieceSnapPoint.h"
#include "Components/ActorComponent.h"
#include "BuildingPieceSpawner.generated.h"

class ABuildingPiece;
class UPlayerInteractionSensor;
class ACustomPlayerController;
class UMaterialInterface;
class UBuildingPieceSnapPoint;


UCLASS( ClassGroup=(BulidingSystem), Blueprintable, meta=(BlueprintSpawnableComponent) )
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
	float SpawnLoopRate = .01;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Building System")
	UMaterialInterface* GoodPlacementMaterial;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Building System")
	UMaterialInterface* BadPlacementMaterial;

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

	UPROPERTY(BlueprintReadOnly,Category="Building System")
	bool bIsOKToPlace;

	/**
	 * @brief Set pointer references from owning character
	 */
	void SetReferences();

	/**
	 * @brief Set input bindings from Custom Player Controller
	 */
	void SetBindings();

	/**
	 * @brief Clear input bindings from Custom Player Controller
	 */
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
	* @brief Checks for snap point during spawn loop
	* @param OutTransform Transform of closest eligible snap point
	 * @param OutSnapPoint Snap point found
	* @param ViewLocation View Location of player
	* @param BuildingPiece Building Piece to check for snaps with
	* @return true if snap point found, false if not
	*/
	bool SnapPointFound(FTransform& OutTransform, UBuildingPieceSnapPoint*& OutSnapPoint, FVector ViewLocation, const ABuildingPiece* BuildingPiece) const;

	
	/**
	 * @brief Checks for snap point during spawn loop
	 * @param OutTransform Transform of closest eligible snap point
	 * @param ViewLocation View Location of player
	 * @param BuildingPiece Building Piece to check for snaps with
	 * @return true if snap point found, false if not
	 */
	bool SnapPointFound(FTransform& OutTransform,FVector ViewLocation, const ABuildingPiece* BuildingPiece) const;

	/**
	 * @brief Takes Location of Ghost mesh, ends spawn loop, and spawns a new finalized version 
	 */
	UFUNCTION()
	void FinalizePlacement();

	/**
	 * @brief Destroys ghost mesh and ends spawn loop
	 */
	UFUNCTION()
	void EndSpawnLoop();


	/**
	 * @brief Helper function to change material of all mesh components of an actor
	 * @param Actor Target Actor
	 * @param NewMaterial  New Material 
	 */
	static void SetMaterial(const AActor* Actor, UMaterialInterface* NewMaterial);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_SpawnBuildingPiece(TSubclassOf<ABuildingPiece> Class, FTransform Transform);
	bool Server_SpawnBuildingPiece_Validate(TSubclassOf<ABuildingPiece> Class, FTransform Transform);
	void Server_SpawnBuildingPiece_Implementation(TSubclassOf<ABuildingPiece> Class, FTransform Transform);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomFoliageManager.generated.h"

class UCustomFoliageISMC;
class UFoliageSwapper;
class ACustomFoliageBase;

UCLASS()
class ENVIRONMENTSYSTEM_API ACustomFoliageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomFoliageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/**
	 * @brief Called from Foliage Swapper on begin play
	 * @param ComponentToTrack 
	 */
	void CheckInComponent(UFoliageSwapper* ComponentToTrack);

	/**
	 * @brief Called from Foliage Swapper when component is destroyed
	 * @param ComponentToRemove 
	 */
	void CheckOutComponent(UFoliageSwapper* ComponentToRemove);

protected:

	/**
	 * @brief Frequency for swap loop 
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Foliage System")
	float SwapLoopRate = .1;

	/**
	 * @brief Range to swap out foliage
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Foliage System")
	float SwapRange = 500;
	
	/**
	 * @brief Array of tracked swapper components
	 */
	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	TArray<UFoliageSwapper*> TrackedComponents;

	/**
	 * @brief Array of Custom Foliage ISMCs.  Set by finding FoliageInstanceActor. 
	 */
	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	TArray<UCustomFoliageISMC*> CustomFoliageISMCs;

	/**
	 * @brief Spawned Custom Foliage Actors in the world
	 */
	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	TArray<ACustomFoliageBase*> TrackedFoliage;
	

	/**
	 * @brief Swap Loop Timer Handle
	 */
	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	FTimerHandle FoliageSwapTimerHandle;

	//bool bSwapFoliage;

	/**
	 * @brief Sets Custom Foliage ISMCs references
	 */
	void GetCustomFoliageISMCs();

	/**
	 * @brief Starts foliage loop timer
	 */
	void StartFoliageSwapLoop();

	/**
	 * @brief Loop function for foliage swapping
	 */
	void FoliageSwapLoop();

	/**
	 * @brief Spawns Foliage Actors which will then remove instances
	 */
	void SwapInstancesInRangeWithActors();

	/**
	 * @brief Removes foliage actors will then add foliage instances
	 */
	void SwapActorsOutRangeWithInstances();

	/**
	 * @brief Helper function to Spawn Foliage Actor
	 */
	void SpawnFoliageActor(FTransform Transform, UCustomFoliageISMC* FoliageISMC);
		
	
};

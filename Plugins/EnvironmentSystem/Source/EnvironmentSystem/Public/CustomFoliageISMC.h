// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "CustomFoliageISMC.generated.h"

class ACustomFoliageBase;

/**
 * 
 */
UCLASS(Blueprintable)
class ENVIRONMENTSYSTEM_API UCustomFoliageISMC : public UFoliageInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Foliage System")
	TSubclassOf<ACustomFoliageBase> FoliageActorClass;
	
	/**
	 * @brief Gets all instances in range
	 * @param Center Center of area to check for instances
	 * @param Range Distances to check for instances
	 * @param Instances Out Instances found
	 */
	void GetInstancesInRange(FVector Center, float Range, TArray<int32>&  Instances) const;

	/**
	 * @brief Get Instances in range of multiple locations
	 * @param Locations Locations to check for instances from
	 * @param Range Range to look for instances
	 * @param Instances Out Instances found
	 */
	void GetInstancesInRangeOfLocations(TArray<FVector> Locations, float Range, TArray<int32>& Instances) const;

	/**
	 * @brief Checks for instance at location and removes it if found
	 * @param Location Location to look for instance 
	 * @return true if found, false if not
	 */
	bool RemoveInstanceAtLocation(FVector Location);

	/**
	 * @brief Get Index from a give location
	 * @param Location Location to look for index
	 * @param OutIndex Index of instance if found
	 * @return True if found, false if not
	 */
	bool GetInstanceIndexFromLocation(FVector Location, int32& OutIndex) const;
	
};

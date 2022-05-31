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
	
	void GetInstancesInRange(FVector Center, float Range, TArray<int32>&  Instances) const;

	void GetInstancesInRangeOfLocations(TArray<FVector> Locations, float Range, TArray<int32>& Instances) const;
	
};

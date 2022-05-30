// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "CustomFoliageISMC.generated.h"

class ACustomFoliageBase;

/**
 * 
 */
UCLASS()
class ENVIRONMENTSYSTEM_API UCustomFoliageISMC : public UFoliageInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Foliage System")
	TSubclassOf<ACustomFoliageBase> FoliageActorClass;
	
};

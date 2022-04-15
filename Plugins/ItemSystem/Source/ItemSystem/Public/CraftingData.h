// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftingData.generated.h"

class AItemBase;

USTRUCT(BlueprintType)
struct FRecipeComponent
{
	GENERATED_USTRUCT_BODY()

	/**
	 * @brief Subclass for type of Recipe Component
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Position")
	TSubclassOf<AItemBase> ComponentClass;

	/**
	 * @brief Quantity of item in Recipe Component
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Position")
	int32 Quantity;

};
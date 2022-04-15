// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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

USTRUCT(BlueprintType)
struct FCraftingRecipe : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	 * @brief Name of Recipe
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Position")
	FName RecipeName;

	/**
	 * @brief Array of items required to craft recipe 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Position")
	TArray<FRecipeComponent> RecipeInputs;

	/**
	 * @brief Array of items that are produced by the recipe 
	 */	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Position")
	TArray<FRecipeComponent> RecipeOutputs;

};
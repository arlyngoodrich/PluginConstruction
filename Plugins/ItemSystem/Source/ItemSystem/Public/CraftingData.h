// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Engine/DataTable.h"
#include "CraftingData.generated.h"

class AItemBase;
class UCraftingComponent;

/**
 * @brief Data Struct for standardized crafting recipe inputs and outputs
 */
USTRUCT(BlueprintType)
struct FRecipeComponent 
{
	GENERATED_USTRUCT_BODY()

	/**
	 * @brief Subclass for type of Recipe Component
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Recipe")
	TSubclassOf<AItemBase> ComponentClass;

	/**
	 * @brief Quantity of item in Recipe Component
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Recipe")
	int32 Quantity;

	/**
	 * @brief Default Constructor for RecipeComponent
	 */
	FRecipeComponent()
	{
		ComponentClass = nullptr;
		Quantity = 0;
	}

	/**
	 * @brief Valid Constructor for crafting recipe
	 */
	FRecipeComponent(TSubclassOf<AItemBase> const SetComponentClass,int32 const SetQuantity)
	{
		ComponentClass = SetComponentClass;
		Quantity = SetQuantity;
	}

	bool operator==(const FRecipeComponent& RecipeComponent) const
	{
		return (ComponentClass == RecipeComponent.ComponentClass);
	}

};

/**
 * @brief Data struct for encapsulating crafting recipes.  Used by crafting components to create new items.  
 */
USTRUCT(BlueprintType)
struct FCraftingRecipe : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	 * @brief Name of Recipe
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Recipe")
	FName RecipeName;

	/**
	 * @brief Array of items required to craft recipe 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Recipe")
	TArray<FRecipeComponent> RecipeInputs;

	/**
	 * @brief Array of items that are produced by the recipe 
	 */	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Recipe")
	TArray<FRecipeComponent> RecipeOutputs;

	/**
	 * @brief Array of Crafting Stations classes that can craft this recipe 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Recipe")
	TArray<TSubclassOf<UCraftingComponent>> EligibleCraftingComponentTypes;

	/**
	 * @brief Default Constructor for Crafting Recipe
	 */
	FCraftingRecipe()
	{
		RecipeName = FName();
	}

	/**
	 * @brief Valid Constructor for crafting Recipe 
	 */
	FCraftingRecipe(const FName SetRecipeName,const TArray<FRecipeComponent> SetRecipeInputs,const TArray<FRecipeComponent> SetRecipeOutputs,
	                const TArray<TSubclassOf<UCraftingComponent>> SetEligibleCraftingComponentTypes)
	{
		RecipeName = SetRecipeName;
		RecipeInputs = SetRecipeInputs;
		RecipeOutputs = SetRecipeOutputs;
		EligibleCraftingComponentTypes = SetEligibleCraftingComponentTypes;
	}

	bool operator==(const FCraftingRecipe& Recipe) const
	{
		return (RecipeName == Recipe.RecipeName);
	}

};
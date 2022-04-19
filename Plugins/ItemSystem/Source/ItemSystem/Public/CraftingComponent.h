// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingData.h"
#include "CraftingComponent.generated.h"

class UInventoryComponent;

/**
 * @brief Base component for creating new items from other items
 */
UCLASS( ClassGroup=(Crafting), Blueprintable, meta=(BlueprintSpawnableComponent) )
class ITEMSYSTEM_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();

	/**
	 * @brief ONLY FOR TESTING. Do not use for gameplay.
	 * @param NewRecipe recipe to add to Eligible crafting array
	 */
	void Debug_AddEligibleRecipe(FCraftingRecipe NewRecipe);

	/**
	 * @return Number of recipes in eligible recipe array
	 */
	int32 GetNumValidRecipes() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRegister() override;
	
	/**
	 * @brief Data table to reference for filling Eligible Crafting Recipe array
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Crafting Data")
	UDataTable* CraftingRecipeTable;

	/**
	 * @brief Eligible Crafting Components that this component can use.
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Crafting Data")
	TArray<FCraftingRecipe> EligibleCraftingRecipes;

	/**
	 * @brief Uses set Crafting Recipe Table reference to fill Eligible Crafting Recipe array
	 */
	void InitializeRecipes();

	/**
	 * @brief Checks a recipe to see if the crafting component is able to craft it
	 * @return True if eligible, false if not
	 */
	bool CanCraftRecipe(FCraftingRecipe RecipeToCheck) const;
};

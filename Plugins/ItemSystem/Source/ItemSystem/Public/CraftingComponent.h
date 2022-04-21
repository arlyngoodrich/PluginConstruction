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

	/**
	 * @brief Uses inventories attached to owning actor to craft a recipe
	 * @param Recipe Recipe to craft
	 * @return Returns true if recipe was crafted, returns false if not
	 */
	bool CraftRecipe(FCraftingRecipe Recipe);

	/**
	 * @brief Gets Crafting Components' eligible crafting recipes
	 */
	UFUNCTION(BlueprintPure,Category="Crafting")
	TArray<FCraftingRecipe> GetEligibleCraftingRecipes();

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
	 * @brief Called when a crafted recipe output cannot be placed into an inventory.  
	 * @param ItemData Item Data that needs to be spawned into the world
	 */
	UFUNCTION(BlueprintNativeEvent,Category="Crafting")
	void SpawnExcessItem(FItemData ItemData);

	/**
	 * @brief Set as true when Recipes are initialized
	 */
	bool bRecipesInitialized;
	
	/**
	 * @brief Uses set Crafting Recipe Table reference to fill Eligible Crafting Recipe array
	 */
	void InitializeRecipes();
	
	/**
	 * @brief Checks a recipe to see if the crafting component is able to craft it
	 * @return True if eligible, false if not
	 */
	bool IsComponentEligibleToCraftRecipe(FCraftingRecipe RecipeToCheck) const;

	/**
	 * @brief Performs checks to see if the recipe can be crafted
	 * @param Recipe Recipe to check 
	 * @return Returns true if recipe can be crafted, returns false if it cannot
	 */
	bool CraftRecipeChecks(FCraftingRecipe Recipe) const;


	/**
	 * @brief Checks inventories to ensure enough quantity of a component is available 
	 * @param Component to to check quantities
	 * @param InventoryComponents Inventories to check  
	 * @return Returns true if enough input component available, returns false if not. 
	 */
	static bool InputComponentCheck(FRecipeComponent Component,TArray<UInventoryComponent*> InventoryComponents);

	/**
	 * @brief Removes recipe component from inventories
	 * @param RecipeComponent Component to consume
	 * @param InventoryComponents Inventories to remove items from
	 * @return return true if components were removed, false if not
	 */
	bool ConsumeComponentInput(FRecipeComponent RecipeComponent,TArray<UInventoryComponent*> InventoryComponents);

	/**
	 * @brief Creates Recipe component output.  Will attempt to add to inventories. If not, then will call blueprint native event
	 * to spawn excess item into world 
	 * @param RecipeOutput Recipe Component to output
	 * @param InventoryComponents Inventory Components to attempt to add item to
	 */
	void DeliverRecipeOutput(FRecipeComponent RecipeOutput, TArray<UInventoryComponent*> InventoryComponents);
	
	/**
	 * @brief Helper function to gets pointers of inventories attached to owning actor
	 * @param OutInventoryComponents inventory pointers
	 * @return True if inventories found, false if not
	 */
	bool GetInventories(TArray<UInventoryComponent*>& OutInventoryComponents) const;
	
};



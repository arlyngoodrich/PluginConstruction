// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingData.h"
#include "CraftingComponent.generated.h"

class UInventoryComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateCraftingUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingStarted, float, CraftingTime);

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
	 * @brief Called when associated inventories are updated
	 */
	UPROPERTY(BlueprintAssignable,Category="Crafting")
	FUpdateCraftingUI CraftingUIUpdate;

	/**
	 * @brief Broadcast when an active recipe has been set
	 */
	UPROPERTY(BlueprintAssignable,Category="Crafting")
	FOnCraftingStarted OnCraftingStarted;

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
	 * @brief Uses inventories attached to owning actor to craft a recipe.  Will perform RPC if not server. 
	 * @param Recipe Recipe to craft
	 * @return Returns true if recipe was crafted, returns false if not
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	bool CraftRecipe(FCraftingRecipe Recipe);

	/**
	 * @brief Gets Crafting Components' eligible crafting recipes
	 */
	UFUNCTION(BlueprintPure,Category="Crafting")
	TArray<FCraftingRecipe> GetEligibleCraftingRecipes();

	/**
	 * @brief Checks attached inventories for 
	 * @param ItemData 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	int32 GetAvailableQtyOfItem(FItemData ItemData) const;

	/**
	 * @brief Checks if enough of component in inventory
	 * @param Recipe Recipe to check
	 * @return True if enough, false if not 
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	bool CanRecipeBeCrafted(FCraftingRecipe Recipe);


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
	 * @brief Set as true when Recipes are initialized
	 */
	bool bRecipesInitialized;

	/**
	 * @brief Broadcast to crafting UI to update
	 */
	UFUNCTION()
	void OnInventoryUpdate();
	
	/**
	 * @brief Pointers to inventory components
	 */
	UPROPERTY()
	TArray<UInventoryComponent*> InventoryComponents;

	
	/**
	 * @brief Timer handle for crafting duration 
	 */
	FTimerHandle CraftingTimerHandle;

	/**
	 * @brief true if there is currently an item that is being crafted
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Crafting")
	bool bIsActivelyCrafting;

	/**
	 * @brief Recipe that is actively being crafted by the component
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Crafting")
	FCraftingRecipe ActiveRecipe;

	/**
	 * @brief Array of Recipes waiting to be crafted
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Crafting")
	TArray<FCraftingQueueSlot> CraftingQueue;


	/**
	* @brief Uses set Crafting Recipe Table reference to fill Eligible Crafting Recipe array
	*/
	void InitializeRecipes();

	/**
	 * @brief Called when a crafted recipe output cannot be placed into an inventory.  
	 * @param ItemData Item Data that needs to be spawned into the world
	 */
	void SpawnExcessItem(FItemData ItemData);

	/**
	 * @brief Called when a new recipe is set
	 */
	UFUNCTION()
	void OnRep_ActiveRecipeSet();
	
	/**
	 * @brief Sets active recipe and starts timer to deliver to inventories
	 * @param Recipe Recipe to set as active recipe
	 */
	void StartCraftingTimer(FCraftingRecipe Recipe);

	/**
	 * @brief Called after crafting timer has finished to deliver output to inventories
	 */
	void FinalizeCrafting();
	
	/**
	 * @brief Adds recipe to crafting queue
	 * @param Recipe Recipe to add to crafting queue
	 * @param RecipeQty Times recipe should be crafted
	 */
	void AddRecipeToQueue(FCraftingRecipe Recipe, int32 RecipeQty);

	/**
	* @brief Attempts to craft next item in queue
	*/
	void CraftFromQueue();

	
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
	 * @param InventoryComponentsRef Inventories to check  
	 * @return Returns true if enough input component available, returns false if not. 
	 */
	static bool InputComponentCheck(FRecipeComponent Component,TArray<UInventoryComponent*> InventoryComponentsRef);

	/**
	 * @brief Removes recipe component from inventories
	 * @param RecipeComponent Component to consume
	 * @param InventoryComponentsRef Inventories to remove items from
	 * @return return true if components were removed, false if not
	 */
	bool ConsumeComponentInput(FRecipeComponent RecipeComponent,TArray<UInventoryComponent*> InventoryComponentsRef);

	/**
	 * @brief Creates Recipe component output.  Will attempt to add to inventories. If not, then will call blueprint native event
	 * to spawn excess item into world 
	 * @param RecipeOutput Recipe Component to output
	 * @param InventoryComponentsRef Inventory Components to attempt to add item to
	 */
	void DeliverRecipeOutput(FRecipeComponent RecipeOutput, TArray<UInventoryComponent*> InventoryComponentsRef);
	
	/**
	 * @brief Helper function to gets pointers of inventories attached to owning actor
	 * @return True if inventories found, false if not
	 */
	bool UpdateInventories();

	/**
	 * @brief Cycles through inventories and binds OnInventoryUpdate method to when inventory updates
	 */
	void UpdateInventoryBindings(TArray<UInventoryComponent*> NewInventoryComponents);

	UFUNCTION(Server,WithValidation,Reliable)
	void Server_RequestCraftRecipe(FCraftingRecipe Recipe);
	bool Server_RequestCraftRecipe_Validate(FCraftingRecipe Recipe);
	void Server_RequestCraftRecipe_Implementation(FCraftingRecipe Recipe);

	UFUNCTION(Client,Reliable)
	void Client_CraftingStarted(float CraftDuration);
	void Client_CraftingStarted_Implementation(float CraftDuration);

	UFUNCTION(Client,Reliable)
	void Client_CraftingFinished();
	void Client_CraftingFinished_Implementation();
};



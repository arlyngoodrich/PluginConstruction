// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftingData.h"
#include "CustomUserWidget.h"
#include "CraftingWidget.generated.h"



class UCraftingComponent;
class UCraftingRecipeWidget;
class UCraftingRecipeComponentWidget;

/**
 * @brief Base class of user widget for crafting UIs.  Handles back end logic for widget. 
 */
UCLASS()
class ITEMSYSTEM_API UCraftingWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	UCraftingWidget();
	
	/**
	 * @brief Owning crafting component.  Should be set before widget construct. 
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting", meta = (ExposeOnSpawn))
	UCraftingComponent* MyCraftingComponent;

	/**
	 * @brief Recipe Widget class to be created.  Default is base class but should be overriden in BP
	 */
	UPROPERTY(EditDefaultsOnly, Category="Crafting")
	TSubclassOf<UCraftingRecipeWidget> CraftingRecipeWidgetClass;

	/**
	 * @brief Recipe Component Widget class to be created.  Default is base class but should be overriden in BP
	 */
	UPROPERTY(EditDefaultsOnly, Category="Crafting")
	TSubclassOf<UCraftingRecipeComponentWidget> CraftingRecipeComponentWidgetClass;
	
	/**
	 * @brief Sets references to variables for widget setup
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetReferences(UCraftingComponent* SetMyCraftingComponent,APlayerController* OwningPlayer);
	

	UFUNCTION(BlueprintCallable, Category="Crafting")
	void UpdateIfRecipesCanBeCrafted();
	
	/**
	 * @brief Updates Input widgets with the available quantity of their input from the parent crafting component
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void UpdateCraftingInputComponentQuantities();
	
	/**
	 * @brief Used when a recipe in the grid is selected to show recipe details in crafting panel 
	 * @param NewActiveRecipe Recipe to set as active
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetActiveRecipe(FCraftingRecipe NewActiveRecipe);

	/**
	 * @brief Clears active recipe reference
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void ClearActiveRecipe();

	/**
	 * @brief Update's quantity of recipe to be added to crafting queue
	 * @param NewQuantity 
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void UpdateCraftingQuantity(int32 NewQuantity);
	
	/**
	 * @brief Gets active recipe as item data.  May be invalid if there is not an active recipe.
	 */
	UFUNCTION(BlueprintPure,Category="Crafting")
	FItemData GetActiveRecipeAsItemData() const;

protected:

	/**
	 * @brief Pointers to Widget recipes created during Initialize Recipes method
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	TArray<UCraftingRecipeWidget*> CraftingRecipeWidgets;

	/**
	 * @brief Pointers to the input widgets created when an active recipe is selected
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	TArray<UCraftingRecipeComponentWidget*> RecipeInputWidgets;
	
	/**
	 * @brief Active recipe that has been selected by the player in the UI
	 */
	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	FCraftingRecipe ActiveRecipe;

	/**
	 * @brief Recipe currently being crafted
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	FCraftingRecipe CurrentlyCraftingRecipe;

	/**
	 * @brief True if recipe is actively being crafted, false if not
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	bool bIsRecipeBeingCrafted;

	/**
	 * @brief Timer handling crafting time
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	FTimerHandle CraftingTimer;

	/**
	 * @brief boolean for if an active recipe is selected.  Should mostly be true except when the menu is first opened
	 * and before a recipe is selected. 
	 */
	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	bool bHasActiveRecipe = false;

	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	int32 CraftingQuantity;

	virtual void BeginDestroy() override;
	
	/**
	 * @brief Creates Recipe Widget Objects for display in recipe grid
	 */
	UFUNCTION(BlueprintCallable, Category="Crafting")
	void InitializeCraftingWidget();

	/**
 * @brief called by binding to On Inventory Update from crafting component
 */
	UFUNCTION()
	void OnInventoryUpdate();

	/**
	 * @brief Updates Crafting Queue Widgets
	 * @param UpdatedQueue Updated queue coming from crafting component
	 */
	UFUNCTION()
	void OnCraftingQueueUpdated(TArray<FCraftingQueueSlot> UpdatedQueue);

	/**
	 * @brief Called after Initialized Recipes methods created Crafting Recipes Widgets.  Should fill grid in BP
	 */
	UFUNCTION(BlueprintImplementableEvent,Category="Crafting",DisplayName="Setup Crafting Grid")
	void BP_UpdateRecipeGrid();

	/**
	 * @brief Called after recipe inputs are refreshed to place the input widgets in the Recipe Details panel 
	 */
	UFUNCTION(BlueprintImplementableEvent,Category="Crafting",DisplayName="Setup Recipe Details")
	void BP_UpdateActiveRecipeDetails();

	UFUNCTION(BlueprintImplementableEvent,Category="Crafting",DisplayName="Update Crafting Queue")
	void BP_UpdateCraftingQueue(const TArray<FCraftingQueueSlot>& UpdatedQueue);

	/**
	 * @brief Called by binding to crafting component 
	 * @param CraftDuration time duration of crafting that is currently being crafted
	 * @param Recipe Recipe that is currently being crafted
	 */
	UFUNCTION()
	void OnNewRecipeCraftStart(float CraftDuration,FCraftingRecipe Recipe);

	/**
	 * @brief Called by the crafting timer after the craft duration is finished
	 */
	UFUNCTION()
	void OnRecipeCraftFinish();

	/**
	 * @brief Empties Crafting recipe widgets array and creates new widget objects from eligible crafting recipes array
	 * from owning crafting component
	 */
	void RefreshRecipeWidgetReferences();


	/**
	 * @brief Clears input widgets
	 */
	void ClearRecipeInputs();

	/**
	 * @brief Empties crafting input widgets and creates new ones based on the current active recipe. 
	 */
	void RefreshRecipeInputWidgetReferences();
	
};

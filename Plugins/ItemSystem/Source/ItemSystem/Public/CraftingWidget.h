// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftingData.h"
#include "CustomUserWidget.h"
#include "CraftingWidget.generated.h"

/**
 * 
 */

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

	/**
	 * @brief Updates Input widgets with the available quantity of their input from the parent crafting component
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void UpdateCraftingInputComponentQuantities();
	
	/**
	 * @brief Used when a recipe in the grid is selected to show recipe details in crafting panel 
	 * @param NewActiveRecipe Recipe to set as active
	 */
	void SetActiveRecipe(FCraftingRecipe NewActiveRecipe);

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
	 * @brief boolean for if an active recipe is selected.  Should mostly be true except when the menu is first opened
	 * and before a recipe is selected. 
	 */
	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	bool bHasActiveRecipe = false;
	
	/**
	 * @brief Creates Recipe Widget Objects for display in recipe grid
	 */
	UFUNCTION(BlueprintCallable, Category="Crafting")
	void InitializeCraftingWidget();

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

	/**
	 * @brief Empties Crafting recipe widgets array and creates new widget objects from eligible crafting recipes array
	 * from owning crafting component
	 */
	void RefreshRecipeWidgetReferences();

	/**
	 * @brief Empties crafting input widgets and creates new ones based on the current active recipe. 
	 */
	void RefreshRecipeInputWidgetReferences();
	
};

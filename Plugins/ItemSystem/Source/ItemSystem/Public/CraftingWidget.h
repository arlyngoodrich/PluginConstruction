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

	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	TArray<UCraftingRecipeComponentWidget*> RecipeInputWidgets;
	
	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	FCraftingRecipe ActiveRecipe;

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

	UFUNCTION(BlueprintImplementableEvent,Category="Crafting",DisplayName="Setup Recipe Details")
	void BP_UpdateActiveRecipeDetails();

	/**
	 * @brief Empties Crafting recipe widgets array and creates new widget objects from eligible crafting recipes array
	 * from owning crafting component
	 */
	void RefreshRecipeWidgetReferences();

	void RefreshRecipeInputWidgetReferences();
	
};

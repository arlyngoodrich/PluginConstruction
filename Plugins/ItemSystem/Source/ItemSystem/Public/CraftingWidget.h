// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "CraftingWidget.generated.h"

/**
 * 
 */

class UCraftingComponent;
class UCraftingRecipeWidget;

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

protected:

	/**
	 * @brief Pointers to Widget recipes created during Initialize Recipes method
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	TArray<UCraftingRecipeWidget*> CraftingRecipeWidgets;

	/**
	 * @brief Creates Recipe Widget Objects for display in recipe grid
	 */
	UFUNCTION(BlueprintCallable, Category="Crafting")
	void InitializeRecipes();

	/**
	 * @brief Called after Initialized Recipes methods created Crafting Recipes Widgets.  Should fill grid in BP
	 */
	UFUNCTION(BlueprintImplementableEvent,Category="Crafting",DisplayName="Setup Crafting Grid")
	void BP_SetupCraftingGrid();

	
	
};

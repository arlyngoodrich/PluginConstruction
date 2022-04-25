// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "CraftingData.h"
#include "CraftingRecipeWidget.generated.h"

class UCraftingComponent;
class UCraftingWidget;

/**
 * @brief  Native class for Crafting Recipe Widget. 
 */
UCLASS()
class ITEMSYSTEM_API UCraftingRecipeWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	/**
	 * @brief Crafting recipe assigned to this widget
	 */
	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	FCraftingRecipe MyCraftingRecipe;

	/**
	 * @brief Pointer to the owning player's crafting component
	 */
	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	UCraftingComponent* MyCraftingComponent;

	/**
	 * @brief Parent Crafting widget
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	UCraftingWidget* MyCraftingWidget;
	
	/**
	 * @brief Called by parent widget to set references before construct
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetReferences(FCraftingRecipe SetMyCraftingRecipe, UCraftingComponent* SetMyCraftingComponent,
	                   UCraftingWidget* SetMyCraftingWidget, APlayerController* OwningPlayer);

protected:
	
	/**
	 * @brief Set during Set References method.  Item Data version of the Recipe's output component
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	FItemData OutputItemData;

	/**
	 * @brief Set During Set Reference method.  Array of Item Data versions of the Recipe's input components. 
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	TArray<FItemData> InputItemData;


	/**
	 * @brief Called when the recipe is clicked in the UI, sets the active recipe in the Crafting Widget
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetActiveRecipe();
	
	/**
	 * @brief Helper function to converting the assigned crafting recipe's inputs and outputs into item data.
	 * Sets OutputItemData and InputItemData variables.  Called from Set References methods. 
	 * @param CraftingRecipe Crafting Recipe to use to convert. 
	 */
	void SetItemDataReferences(FCraftingRecipe CraftingRecipe);
	
};

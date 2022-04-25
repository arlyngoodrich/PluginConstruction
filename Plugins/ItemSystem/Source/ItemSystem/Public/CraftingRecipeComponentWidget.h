// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "CraftingData.h"
#include "CraftingRecipeComponentWidget.generated.h"

class UCraftingComponent;

/**
 * @brief Native class for displaying recipe inputs in the Recipe Details panel of the crafting widget.  
 */
UCLASS()
class ITEMSYSTEM_API UCraftingRecipeComponentWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	/**
	 * @brief Assigned recipe component
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	FRecipeComponent MyRecipeComponent;

	/**
	 * @brief Owning actor's crafting component
	 */
	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	UCraftingComponent* MyCraftingComponent;

	/**
	 * @brief Set key parameters before widget construct
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetReferences(FRecipeComponent SetMyRecipeComponent,UCraftingComponent* SetMyCraftingComponent,APlayerController* OwningPlayer);

	/**
	 * @brief Item data version of recipe component
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	FItemData ComponentItemData;

	/**
	 * @brief Quantity of recipe component in the available inventories
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	int32 ItemQtyInInventories;
	
	/**
	 * @brief Quantity needed to craft item.
	 */
	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	int32 ItemQtyNeeded;




};

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
	 * @brief Set key parameters before widget construct
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetReferences(FRecipeComponent SetMyRecipeComponent,UCraftingComponent* SetMyCraftingComponent,APlayerController* OwningPlayer);

	/**
	 * @brief Called by Set References and Set Inventory Quantities method to update to icon appearance
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Crafting")
	void UpdateAppearance();

	/**
	 * @brief Called by Crafting Component when the recipe input should update the qty of items in the associated
	 * inventories
	 * @param SetItemQtyInInventories New available quantities 
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetInventoryQuantities(int32 SetItemQtyInInventories);

	/**
	 * @brief Called by crafting component when the crafting amount changes.  Multiples recipe input to show how much is
	 * needed to craft
	 * @param NewCraftingAmount Amount of recipe that is to be crafted
	 */
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetInputQuantityNeeded(int32 NewCraftingAmount);
	
	/**
	 * @brief Returns Recipe component reference
	 */
	UFUNCTION(BlueprintPure, Category="Crafting")
	FRecipeComponent GetRecipeComponent() const;

	/**
	 * @brief Returns item data version of Recipe Component
	 */
	UFUNCTION(BlueprintPure, Category="Crafting")
	FItemData GetComponentItemData() const;


protected:

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

	/**
	 * @brief If there is enough inputs in the inventory.  Updated when Item Quantity in Inventories is updated.  
	 */
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	bool bHasEnoughQuantity;


};

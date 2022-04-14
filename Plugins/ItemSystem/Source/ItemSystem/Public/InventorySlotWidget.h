// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "InventoryData.h"
#include "InventorySlotWidget.generated.h"

class UInventoryGridWidget;
class UInventoryComponent;

/**
 * @brief Base User Widget used for Inventory Grid Slot Widgets.  Handles back end native coding. 
 */
UCLASS()
class ITEMSYSTEM_API UInventorySlotWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Inventory Slot reference. Set by inventory grid widget when slot is created. 
	 */
	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	FInventorySlot MyInventorySlot;

	/**
	 * @brief Owning Grid Widget reference. Set by inventory grid widget when slot is created. 
	 */
	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UInventoryGridWidget* OwningGridWidget;

	/**
	 * @brief Owning inventory component reference.  Set by inventory grid widget when slot is created. 
	 */
	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UInventoryComponent* OwningInventory;

	/**
	 * @brief Boolean for if the slot is being dragged over.  Set in Inventory Grid when handling drag operation.  
	 */
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	bool bDraggedOver;
	
	/**
	 * @brief Called when a item widget is dragged over slots.  Used to trigger Inventory Grid drag over function
	 * @param DraggedOverItem Item that is being dragged over slot
	 * @param bRotateItem If item should be rotated or not
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void OnSlotDraggedOver(FItemData DraggedOverItem,bool bRotateItem);

	/**
	 * @brief When item is dropped onto inventory 
	 * @param OriginatingInventory The inventory the item originated from.  May be same as slot -- will trigger a move
	 * if different.  Will be a transfer if inventories are different. 
	 * @param DroppedItemData Item dropped on slot
	 * @param bRotateItem If item should be rotated or not.
	 * @return 
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool OnItemDropped(UInventoryComponent* OriginatingInventory, FInventoryItemData DroppedItemData, bool bRotateItem);

};

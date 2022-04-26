// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "InventoryData.h"
#include "InventoryItemWidget.generated.h"


class UInventoryGridWidget;
class UInventorySlotWidget;
class UInventoryComponent;



/**
 * @brief Base User Widget used for Item Widgets.  Handles back end native coding. 
 */
UCLASS()
class ITEMSYSTEM_API UInventoryItemWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Inventory Item Data struct from the owning item.  Should be set before construct. 
	 */
	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	FInventoryItemData MyInventoryItemData;
	
	/**
	 * @brief Owning Grid Widget.  Should be set before construct. 
	 */
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	UInventoryGridWidget* OwningGridWidget;
	
	/**
	 * @brief Owning Widget Slot used to set position.  Should be set before construct.  
	 */
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	UInventorySlotWidget* OwningSlot;
	
	/**
	 * @brief Owning Owning Inventory Component.  Should be set before construct.  
	 */
	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UInventoryComponent* OwningInventory;

	/**
	 * @brief When used as a component, parent widgets can call to set the references for this widget
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void SetReferences(FInventoryItemData SetMyInventoryItemData, UInventoryGridWidget* SetOwningGridWidget,
	                   UInventorySlotWidget* SetOwningSlot,UInventoryComponent* SetOwningInventory,APlayerController* OwningPlayer);

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
protected:
	
	/**
	 * @brief Checks to see if item can move into position.
	 * @param TargetPosition Position to move item to. 
	 * @param bShouldRotate If the item should be rotated
	 * @return returns true if Ok, false if not.
	 */
	UFUNCTION(BlueprintPure,Category="Inventory")
	bool CanItemMoveToPosition(FInventory2D TargetPosition, bool bShouldRotate) const;

	/**
	 * @brief Checks if item can transfer to position
	 * @param TargetInventory Inventory to transfer to
	 * @param TargetPosition Position to transfer item to in target inventory
	 * @param bShouldRotate If item should be rotated
	 * @return returns true if Ok, false if not
	 */
	UFUNCTION(BlueprintPure,Category="Inventory")
	bool CanItemTransferToPosition(UInventoryComponent* TargetInventory, FInventory2D TargetPosition,
	                               bool bShouldRotate) const;

	/**
	 * @brief Move item target position
	 * @param TargetPosition Position to move item to
	 * @param bShouldRotate If item should be rotated
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void MoveItem(FInventory2D TargetPosition, bool bShouldRotate) const;
	
	/**
	 * @brief Split item into new stack
	 * @param TargetItemData Target item stack to split
	 * @param NewStackQuantity Quantity for new item stack 
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void SplitItem(FInventoryItemData TargetItemData, int32 NewStackQuantity) const;

	/**
	 * @brief Combines the originating stack into the target stack.  Will remove the originating stack if fully combined
	 * into the target stack or will update the originating stack if partially combined.  Only works for combining stacks
	 * of the same inventory
	 * @param OriginatingStack stack that is instigating the combining action 
	 * @param TargetStack stack that will receive the originating stack 
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool CombineStacks_SameInventory(FInventoryItemData OriginatingStack, FInventoryItemData TargetStack);
	
};

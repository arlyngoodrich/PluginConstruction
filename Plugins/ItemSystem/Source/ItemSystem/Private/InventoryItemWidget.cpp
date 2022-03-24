// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

#include "InventoryComponent.h"
#include "InventoryGridWidget.h"

void UInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if(OwningGridWidget)
	{
		OwningGridWidget->OnItemDragStart(this);
	}

	
}

void UInventoryItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	if(OwningGridWidget)
	{
		OwningGridWidget->OnItemDragCancel(this);
	}
	
}

bool UInventoryItemWidget::CanItemMoveToPosition(const FInventory2D TargetPosition, const bool bShouldRotate) const
{
	FItemData ItemData = MyInventoryItemData.Item;

	if(bShouldRotate)
	{
		ItemData.Rotate();
	}

	if(OwningInventory)
	{
		return OwningInventory->CheckIfItemFits(ItemData,TargetPosition);
	}

	return false;
	
}

bool UInventoryItemWidget::CanItemTransferToPosition(UInventoryComponent* TargetInventory, const FInventory2D TargetPosition,
                                                     const bool bShouldRotate) const
{
	FItemData ItemData = MyInventoryItemData.Item;

	if(bShouldRotate)
	{
		ItemData.Rotate();
	}

	if(TargetInventory)
	{
		return TargetInventory->CheckIfItemFits(ItemData,TargetPosition);
	}

	return false;
	
}

void UInventoryItemWidget::MoveItem(const FInventory2D TargetPosition, const bool bShouldRotate) const
{
	if(OwningInventory)
	{
		OwningInventory->MoveItem(MyInventoryItemData,TargetPosition,bShouldRotate);
	}
}

void UInventoryItemWidget::SplitItem(const FInventoryItemData TargetItemData, const int32 NewStackQuantity) const
{
	if(OwningInventory)
	{
		OwningInventory->SplitItem(TargetItemData,NewStackQuantity);
	}
}

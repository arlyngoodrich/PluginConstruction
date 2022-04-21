// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

#include "InventoryComponent.h"
#include "InventoryGridWidget.h"
#include "ItemSystem.h"

void UInventoryItemWidget::SetReferences(const FInventoryItemData SetMyInventoryItemData,
                                         UInventoryGridWidget* SetOwningGridWidget, UInventorySlotWidget* SetOwningSlot)
{
	MyInventoryItemData = SetMyInventoryItemData;
	OwningGridWidget = SetOwningGridWidget;
	OwningSlot = SetOwningSlot;
}

void UInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UE_LOG(LogItemSystem,Log,TEXT("Native Drag Called"))
	
	if(OwningGridWidget)
	{
		OwningGridWidget->OnItemDragStart(this);
	}

	
}

void UInventoryItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	UE_LOG(LogItemSystem,Log,TEXT("Native Drag Cancel Called"))
	
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

		UE_LOG(LogItemSystem,Log,TEXT("UI for %s item in %s is requesting split"),
			*TargetItemData.Item.DisplayName.ToString(),*OwningInventory->GetOwner()->GetName())
		
		OwningInventory->SplitItem(TargetItemData,NewStackQuantity);
	}
	else
	{
		UE_LOG(LogItemSystem,Warning,TEXT("UI for %s item is requesting split but has null inventory"),
			*TargetItemData.Item.DisplayName.ToString())
	}
}

bool UInventoryItemWidget::CombineStacks_SameInventory(FInventoryItemData const OriginatingStack,
	FInventoryItemData const TargetStack)
{
	if(OwningInventory == nullptr){return false;}

	if(OwningInventory->CombineStacks_SameInventory_Checks(OriginatingStack,TargetStack) == false)
	{
		return false;
	}
	
	OwningInventory->CombineStacks_SameInventory(OriginatingStack,TargetStack);
	return true;
}

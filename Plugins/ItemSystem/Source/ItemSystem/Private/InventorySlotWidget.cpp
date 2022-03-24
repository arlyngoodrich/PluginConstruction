// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "InventoryComponent.h"
#include "InventoryGridWidget.h"


void UInventorySlotWidget::OnSlotDraggedOver(const FItemData DraggedOverItem) const
{
	if(OwningGridWidget != nullptr)
	{
		OwningGridWidget->SetSlotsOnDragOver(MyInventorySlot.Position,DraggedOverItem);
	}
}

bool UInventorySlotWidget::OnItemDropped(const FInventoryItemData DroppedItemData) const
{
	if(OwningInventory != nullptr)
	{
		if(OwningInventory->CheckItemMove(DroppedItemData,MyInventorySlot.Position,false))
		{
			OwningInventory->MoveItem(DroppedItemData,MyInventorySlot.Position,false);
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

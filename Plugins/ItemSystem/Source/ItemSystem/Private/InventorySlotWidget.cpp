// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "InventoryComponent.h"
#include "InventoryGridWidget.h"
#include "ItemSystem.h"


void UInventorySlotWidget::OnSlotDraggedOver(FItemData DraggedOverItem, const bool bRotateItem)
{
	if(OwningGridWidget != nullptr)
	{

		//This will only rotate the item reference from the dragged widget, not the actual item in the inventory
		if(bRotateItem)
		{
			DraggedOverItem.Rotate();
		}
		
		OwningGridWidget->SetSlotsOnDragOver(MyInventorySlot.Position,DraggedOverItem);
	}
}

bool UInventorySlotWidget::OnItemDropped(const FInventoryItemData DroppedItemData, const bool bRotateItem)
{
	if(OwningInventory != nullptr)
	{
		if(OwningInventory->CheckItemMove(DroppedItemData,MyInventorySlot.Position,bRotateItem))
		{
			UE_LOG
				(
					LogItemSystem,Log,TEXT("%s item dropped on pos: %s.  Requesting move in %s inventory"),
					*DroppedItemData.Item.DisplayName.ToString(), *MyInventorySlot.Position.GetPositionAsString(),
					*OwningInventory->GetOwner()->GetName()
				)
			
			OwningInventory->MoveItem(DroppedItemData,MyInventorySlot.Position,bRotateItem);
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

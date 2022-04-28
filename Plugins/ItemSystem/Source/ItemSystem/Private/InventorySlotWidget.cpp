// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "InventoryComponent.h"
#include "InventoryGridWidget.h"


void UInventorySlotWidget::SetReferences(const FInventorySlot SetMyInventorySlot, UInventoryGridWidget* SetOwningGridWidget,
                                         UInventoryComponent* SetOwningInventory,APlayerController* OwningPlayer)
{
	MyInventorySlot = SetMyInventorySlot;
	OwningGridWidget = SetOwningGridWidget;
	OwningInventory = SetOwningInventory;
	SetOwningPlayer(OwningPlayer);
}

void UInventorySlotWidget::OnSlotDraggedOver(FItemData DraggedOverItem, const bool bRotateItem)
{
	if(OwningGridWidget != nullptr)
	{

		//This will only rotate the item reference from the dragged widget, not the actual item in the inventory
		if(bRotateItem)
		{
			DraggedOverItem.Rotate();
		}
		
		OwningGridWidget->SetSlotsOnDragOver(MyInventorySlot.Position,DraggedOverItem,bRotateItem);
	}
}

bool UInventorySlotWidget::OnItemDropped(UInventoryComponent* OriginatingInventory,const FInventoryItemData DroppedItemData, const bool bRotateItem)
{
	if(OwningInventory != nullptr)
	{

		//Check to see if it's being dropped onto the same position
		if(DroppedItemData.StartPosition == MyInventorySlot.Position && OriginatingInventory == OwningInventory)
		{
			return false;
		}

		//If Originating Inventory is null or it is the same as the owning inventory, attempt to do move.  
		if(OriginatingInventory == nullptr || OriginatingInventory == OwningInventory)
		{
			
			if(OwningInventory->CheckItemMove(DroppedItemData,MyInventorySlot.Position,bRotateItem))
			{
				OwningInventory->MoveItem(DroppedItemData,MyInventorySlot.Position,bRotateItem);
				return true;
			}
		}
		else
		{
			
			//If the originating inventory is not null AND it is not equal to the owning inventory, try to do transfer
			if(OwningInventory->CheckItemMove(DroppedItemData,MyInventorySlot.Position,bRotateItem))
			{
				OriginatingInventory->TransferItemToPosition(OwningInventory,MyInventorySlot.Position,DroppedItemData,bRotateItem);
				return true;
			}
		}

		OwningGridWidget->RefreshGrid();
		
		return false;
	}

	return false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGridWidget.h"

#include "InventoryComponent.h"
#include "InventoryData.h"
#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"
#include "ItemSystem.h"

void UInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeGrid();
	InitializeItems();
	
}

void UInventoryGridWidget::InitializeGrid()
{
	if(OwningInventoryComponent == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to creat %s but Owning Inventory Is Null"),
			*GetName())
		return;
	}

	SlotWidgets.Empty();

	const TArray<FInventorySlot>InventorySlots = OwningInventoryComponent->GetInventorySlots();

	for (int i = 0; i < InventorySlots.Num(); ++i)
	{
		
		UInventorySlotWidget* NewSlotWidget = Cast<UInventorySlotWidget>
		(CreateWidget(this, UInventorySlotWidget::StaticClass(), "InventorySlot"));

		if(NewSlotWidget != nullptr)
		{
			NewSlotWidget->MyInventorySlot = InventorySlots[i];
			SlotWidgets.Add(NewSlotWidget);
		}
	}

	BP_SetSlotWidgetsInGrid(SlotWidgets);
}

void UInventoryGridWidget::InitializeItems()
{
	if(OwningInventoryComponent == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to creat %s but Owning Inventory Is Null"),
			*GetName())
		return;
	}

	ItemWidgets.Empty();

	const TArray<FInventoryItemData>InventoryItemData = OwningInventoryComponent->GetInventoryItemData();

	for (int i = 0; i < InventoryItemData.Num(); ++i)
	{
		UInventoryItemWidget* NewItemWidget = Cast<UInventoryItemWidget>
		(CreateWidget(this,UInventoryItemWidget::StaticClass(),"InventoryItem"));
		if(NewItemWidget != nullptr)
		{
			NewItemWidget->MyInventoryItemData = InventoryItemData[i];
			NewItemWidget->OwningGridWidget = this;
			NewItemWidget->OwningInventory = OwningInventoryComponent;

			UInventorySlotWidget* NewOwningSlot;
			if(GetSlotWidgetFromPosition(InventoryItemData[i].StartPosition,NewOwningSlot))
			{
				NewItemWidget->OwningSlot = NewOwningSlot;
			}
			else
			{
				UE_LOG(LogItemSystem,Error,TEXT("Could not add a owning slot to %s item in %s"),
					*InventoryItemData[i].Item.DisplayName.ToString(),*GetName())
			}

			ItemWidgets.Add(NewItemWidget);
		}
	}	
}

bool UInventoryGridWidget::GetSlotWidgetFromPosition(const FInventory2D Position, UInventorySlotWidget*& OutSlotWidget)
{
	if(SlotWidgets.Num() == 0)
	{
		UE_LOG(LogItemSystem,Warning,TEXT("%s is attempting to get postion from slot widget without widgets in array"),
			*GetName())
		return false;
	}

	for (int i = 0; i < SlotWidgets.Num(); ++i)
	{
		UInventorySlotWidget* TargetSlotWidget = SlotWidgets[i];

		if(TargetSlotWidget != nullptr)
		{
			if(TargetSlotWidget->MyInventorySlot.Position == Position)
			{
				OutSlotWidget = TargetSlotWidget;
				return true;
			}
		}
	}

	return false;
}

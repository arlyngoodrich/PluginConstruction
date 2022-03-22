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

	OwningInventoryComponent->OnInventorySlotUpdate.AddDynamic(this,&UInventoryGridWidget::OnInventorySlotUpdate);
	OwningInventoryComponent->OnInventoryUpdate.AddDynamic(this,&UInventoryGridWidget::OnInventoryItemUpdates);
	
}

void UInventoryGridWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ClearItemWidgets();
	SlotWidgets.Empty();

	OwningInventoryComponent->OnInventorySlotUpdate.RemoveDynamic(this,&UInventoryGridWidget::OnInventorySlotUpdate);
	OwningInventoryComponent->OnInventoryUpdate.RemoveDynamic(this,&UInventoryGridWidget::OnInventoryItemUpdates);
}



void UInventoryGridWidget::InitializeGrid()
{
	if(OwningInventoryComponent == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to creat %s but Owning Inventory Is Null"),
			*GetName())
		return;
	}

	ClearSlotWidgets();

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

	BP_SetSlotWidgetsInGrid();
}

void UInventoryGridWidget::InitializeItems()
{
	if(OwningInventoryComponent == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to creat %s but Owning Inventory Is Null"),
			*GetName())
		return;
	}

	ClearItemWidgets();

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

			//Add to viewport?
			//Set position?
		}
	}

	BP_SetItemWidgetsInGrid();
}

void UInventoryGridWidget::OnInventorySlotUpdate()
{
	TArray<FInventorySlot> NewSlotData = OwningInventoryComponent->GetInventorySlots();

	//Update occupied slots for inventory widgets 
	for (int i = 0; i < NewSlotData.Num(); ++i)
	{
		UInventorySlotWidget* TargetSlotWidget;
		if(GetSlotWidgetFromPosition(NewSlotData[i].Position,TargetSlotWidget))
		{
			TargetSlotWidget->MyInventorySlot.bIsOccupied = NewSlotData[i].bIsOccupied;
		}
	}
}

void UInventoryGridWidget::OnInventoryItemUpdates()
{
	//Reset items in inventory
	InitializeItems();
}

void UInventoryGridWidget::ClearItemWidgets()
{
	for (int i = 0; i < ItemWidgets.Num(); ++i)
	{
		UInventoryItemWidget* TargetWidget = ItemWidgets[i];
		if(TargetWidget)
		{
			TargetWidget->RemoveFromParent();
		}
	}

	ItemWidgets.Empty();
}

void UInventoryGridWidget::ClearSlotWidgets()
{
	for (int i = 0; i < SlotWidgets.Num(); ++i)
	{
		UInventorySlotWidget* TargetWidget = SlotWidgets[i];
		if(TargetWidget)
		{
			TargetWidget->RemoveFromParent();
		}
	}

	SlotWidgets.Empty();
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

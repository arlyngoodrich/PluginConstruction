// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGridWidget.h"

#include "InventoryComponent.h"
#include "InventoryData.h"
#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"
#include "ItemSystem.h"

UInventoryGridWidget::UInventoryGridWidget()
{
	SlotWidgetClass = UInventorySlotWidget::StaticClass();
	ItemWidgetClass = UInventoryItemWidget::StaticClass();
}

void UInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
		
}

void UInventoryGridWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ClearItemWidgets();
	SlotWidgets.Empty();

	OwningInventoryComponent->OnInventorySlotUpdate.RemoveDynamic(this,&UInventoryGridWidget::OnInventorySlotUpdate);
	OwningInventoryComponent->OnInventoryUpdate.RemoveDynamic(this,&UInventoryGridWidget::OnInventoryItemUpdates);
	
}


void UInventoryGridWidget::InitializeInventory()
{
    
    UE_LOG(LogItemSystem,Log,TEXT("Inventory Widget Initialized"))

	InitializeGrid();
	InitializeItems();

	OwningInventoryComponent->OnInventorySlotUpdate.AddDynamic(this,&UInventoryGridWidget::OnInventorySlotUpdate);
	OwningInventoryComponent->OnInventoryUpdate.AddDynamic(this,&UInventoryGridWidget::OnInventoryItemUpdates);

	BP_SetSlotWidgetsInGrid();
	BP_SetItemWidgetsInGrid();
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
		(CreateWidget(GetOwningPlayer(), SlotWidgetClass));

		if(NewSlotWidget != nullptr)
		{
			NewSlotWidget->MyInventorySlot = InventorySlots[i];
			UE_LOG(LogItemSystem,Log,TEXT("Inventoey Slot Added with Position: %s "),
				*InventorySlots[i].Position.GetPositionAsString())
			SlotWidgets.Add(NewSlotWidget);
		}
	}
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
		(CreateWidget(GetOwningPlayer(),ItemWidgetClass));
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

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

void UInventoryGridWidget::SetSlotsOnDragOver(const FInventory2D DragPosition, const FItemData DraggedItem)
{

	FInventoryItemData DraggedInventoryItemData;
	bool bOKToPlace;
	
	//Check to see if item is from owning inventory
	if(OwningInventoryComponent->IsItemInInventory(DraggedItem,DraggedInventoryItemData))
	{
		bOKToPlace = OwningInventoryComponent->CheckItemMove(DraggedInventoryItemData,DragPosition,false);
		
	}
	else
	{
		bOKToPlace = OwningInventoryComponent->CheckIfItemFits(DraggedItem,DragPosition);
	}

	//Reused Dragged Inventory Item Data to generate covered slots
	DraggedInventoryItemData = FInventoryItemData(DragPosition,DraggedItem);
	TArray<FInventory2D> CoveredSlots = DraggedInventoryItemData.GetCoveredSlots();
	
	//Set All to false for drag over
	for (int i = 0; i < SlotWidgets.Num(); ++i)
	{
		SlotWidgets[i]->bDraggedOver = false;
	}

	//Set slots that are hovered over as hovered
	for (int i = 0; i < CoveredSlots.Num(); ++i)
	{
		const FInventory2D TargetSlot = CoveredSlots[i];
		UInventorySlotWidget* TargetSlotWidget;
		if(GetSlotWidgetFromPosition(TargetSlot,TargetSlotWidget))
		{
			TargetSlotWidget->bDraggedOver = true;
			TargetSlotWidget->bOnDragOKToPlace = bOKToPlace;
		}
	}
}

void UInventoryGridWidget::OnItemDragStart(const UInventoryItemWidget* InventoryItemWidget)
{
	const TArray<FInventory2D> CoveredPositions = InventoryItemWidget->MyInventoryItemData.GetCoveredSlots();

	for (int i = 0; i < CoveredPositions.Num(); ++i)
	{	
		UInventorySlotWidget* TargetSlot;
		if(GetSlotWidgetFromPosition(CoveredPositions[i],TargetSlot))
		{
			TargetSlot->MyInventorySlot.bIsOccupied = false;
		}
	}
}

void UInventoryGridWidget::OnItemDragCancel(const UInventoryItemWidget* InventoryItemWidget)
{
	const TArray<FInventory2D> CoveredPositions = InventoryItemWidget->MyInventoryItemData.GetCoveredSlots();

	for (int i = 0; i < CoveredPositions.Num(); ++i)
	{	
		UInventorySlotWidget* TargetSlot;
		if(GetSlotWidgetFromPosition(CoveredPositions[i],TargetSlot))
		{
			TargetSlot->MyInventorySlot.bIsOccupied = true;
		}
	}

	//Tell all slots that no longer dragged over
	for (int i = 0; i < SlotWidgets.Num(); ++i)
	{
		SlotWidgets[i]->bDraggedOver = false;
	}

	OnInventoryItemUpdates();
	OnInventorySlotUpdate();
}

void UInventoryGridWidget::AllowTransfer(UInventoryComponent* TargetInventoryComponent)
{
	if(TargetInventoryComponent == nullptr){return;}

	bTransferAvailable = true;
	TransferInventory = TargetInventoryComponent;
}

void UInventoryGridWidget::RemoveTransfer()
{
	bTransferAvailable = false;
	TransferInventory = nullptr;
}

void UInventoryGridWidget::RequestTransfer(const FInventoryItemData TargetItem) const
{
	if(TransferInventory == nullptr){return;}

	OwningInventoryComponent->TransferItem(TransferInventory,TargetItem);
	
}

void UInventoryGridWidget::SetReferences(UInventoryComponent* SetOwningInventoryComponent, APlayerController* OwningPlayer)
{
	OwningInventoryComponent = SetOwningInventoryComponent;
	SetOwningPlayer(OwningPlayer);
	InitializeGrid();
}

void UInventoryGridWidget::RefreshGrid()
{
	OnInventorySlotUpdate();
	OnInventoryItemUpdates();
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

	if(OwningInventoryComponent)
	{
		OwningInventoryComponent->OnInventorySlotUpdate.RemoveDynamic(this,&UInventoryGridWidget::OnInventorySlotUpdate);
		OwningInventoryComponent->OnInventoryUpdate.RemoveDynamic(this,&UInventoryGridWidget::OnInventoryItemUpdates);
	}

	
}


void UInventoryGridWidget::InitializeInventory()
{
    
   if(OwningInventoryComponent==nullptr)
   {
		UE_LOG(LogItemSystem,Warning,TEXT("%s inventory grid widget attemped to initalize without an owning inventory"),
			*GetOwningPlayer()->GetName())
   		return;
   }

	InitializeGrid();
	InitializeItems();

	OwningInventoryComponent->OnInventorySlotUpdate.AddDynamic(this,&UInventoryGridWidget::OnInventorySlotUpdate);
	OwningInventoryComponent->OnInventoryUpdate.AddDynamic(this,&UInventoryGridWidget::OnInventoryItemUpdates);

	BP_SetSlotWidgetsInGrid();
	BP_SetItemWidgetsInGrid();

	//UE_LOG(LogItemSystem,Log,TEXT("Inventory Widget Initialized"))
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
			NewSlotWidget->SetReferences(InventorySlots[i],this,OwningInventoryComponent,GetOwningPlayer());
			
			UE_LOG(LogItemSystem,Verbose,TEXT("Inventoey Slot Added with Position: %s "),
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
		
			UInventorySlotWidget* NewOwningSlot = nullptr;
			if(GetSlotWidgetFromPosition(InventoryItemData[i].StartPosition,NewOwningSlot) == false)
			{
				UE_LOG(LogItemSystem,Error,TEXT("Could not add a owning slot to %s item in %s"),
					*InventoryItemData[i].Item.DisplayName.ToString(),*GetName())
			}

			NewItemWidget->SetReferences(InventoryItemData[i],this,
				NewOwningSlot,OwningInventoryComponent,GetOwningPlayer());

			ItemWidgets.Add(NewItemWidget);

		}
	}
	
}

void UInventoryGridWidget::OnInventorySlotUpdate()
{
	TArray<FInventorySlot> NewSlotData = OwningInventoryComponent->GetInventorySlots();

	UE_LOG(LogItemSystem,Log,TEXT("Update Inventory Slots called for %s"),
		*OwningInventoryComponent->GetOwner()->GetName())

	//Update occupied slots for inventory widgets 
	for (int i = 0; i < NewSlotData.Num(); ++i)
	{
		UInventorySlotWidget* TargetSlotWidget;
		if(GetSlotWidgetFromPosition(NewSlotData[i].Position,TargetSlotWidget))
		{
			TargetSlotWidget->MyInventorySlot.bIsOccupied = NewSlotData[i].bIsOccupied;
			TargetSlotWidget->bDraggedOver = false;
		}
	}
}

void UInventoryGridWidget::OnInventoryItemUpdates()
{
	//Reset items in inventory
	InitializeItems();
	BP_SetItemWidgetsInGrid();
}

void UInventoryGridWidget::ClearItemWidgets()
{
	for (int i = 0; i < ItemWidgets.Num(); ++i)
	{
		if(UInventoryItemWidget* TargetWidget = ItemWidgets[i])
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
		if(UInventorySlotWidget* TargetWidget = SlotWidgets[i])
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

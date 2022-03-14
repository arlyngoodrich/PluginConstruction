// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	InventoryName = FText::FromString("Inventory");
	InventorySize = FInventory2D(2,2);
	MaxWeight = 10.f;
	
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventorySlots);
	DOREPLIFETIME(UInventoryComponent, InventoryItems);
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeSlots();

	// ...
	
}

void UInventoryComponent::InitializeSlots()
{
	InventorySlots.Empty();
	
	for (int X = 0; X < InventorySize.X; ++X)
	{
		for (int Y = 0; Y < InventorySize.Y; ++Y)
		{
			FInventorySlot NewSlot = FInventorySlot(FInventory2D(X,Y),false);
			InventorySlots.Add(NewSlot);
		}
	}

	UE_LOG(LogItemSystem,Log,TEXT("%s inventory slots initalized"),*GetOwner()->GetName());
}

void UInventoryComponent::SetInventory(TArray<FInventoryItemData> InInventoryItems)
{
	InitializeSlots();

	TArray<bool> ItemAddedChecks;

	//Cycle through all items to add
	for (int i = 0; i < InInventoryItems.Num(); ++i)
	{
		const FInventoryItemData NewItem = InInventoryItems[i];
		bool WasItemAdded = AddItemToPosition(NewItem.Item,NewItem.StartPosition);
		
		ItemAddedChecks.Add(WasItemAdded);
	}

	if(ItemAddedChecks.Contains(false))
	{
		UE_LOG(LogItemSystem,Error,TEXT("Failed to Set Inventory for %s"),*GetOwner()->GetName());
	}
}

bool UInventoryComponent::AddItemToPosition(const FItemData Item, const FInventory2D Position)
{

	FInventorySlot Slot;
	const bool isValidSlot = FindSlotAtPosition(Position,Slot);
	
	if(isValidSlot == false || Slot.bIsOccupied == true)
	{
		return false;
	}
	else
	{
		if(CheckIfItemFits(Item,Slot.Position))
		{
			const FInventoryItemData NewInventoryItem = FInventoryItemData(Slot.Position,Item);

			if(SetSlotStatuses(NewInventoryItem.GetCoveredSlots(),true))
			{
				InventoryItems.Add(NewInventoryItem);
				return true;
			}
			else
			{
				return false;
			}
			
		
		}
		else
		{
			return false;
		}
	}
}

bool UInventoryComponent::SetSlotStatus(FInventory2D TargetPosition, const bool NewIsOccupied)
{

	int32 SlotIndex;
	if(FindSlotAtPosition(TargetPosition,SlotIndex))
	{
		InventorySlots[SlotIndex].bIsOccupied = NewIsOccupied;
		UE_LOG(LogItemSystem,Log,TEXT("%s inventory updated slot %s to %s"),
			*GetOwner()->GetName(),*TargetPosition.GetPositionAsString(), NewIsOccupied? "true" : "false")
		return true;
	}
	else
	{
		UE_LOG(LogItemSystem,Warning,TEXT("Could not find %s in %s inventory to set status"),
			*TargetPosition.GetPositionAsString(),*GetOwner()->GetName())
		return false;
	}
}

bool UInventoryComponent::SetSlotStatuses(TArray<FInventory2D> TargetSlotPositions, bool NewIsOccupied)
{

	TArray<bool> SlotChecks;
	
	for (int i = 0; i < TargetSlotPositions.Num(); ++i)
	{
		bool SlotCheck = SetSlotStatus(TargetSlotPositions[i], NewIsOccupied);
		SlotChecks.Add(SlotCheck);
	}

	if(SlotChecks.Contains(false))
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool UInventoryComponent::CheckIfItemFits(const FItemData ItemData, const FInventory2D TargetPosition)
{
	const FInventoryItemData InventoryItemData = FInventoryItemData(TargetPosition,ItemData);
	const TArray<FInventory2D> CoveredSlots = InventoryItemData.GetCoveredSlots();

	for (int i = 0; i < CoveredSlots.Num(); ++i)
	{
		const FInventory2D CoveredSlotPosition = FInventory2D(CoveredSlots[i].X,CoveredSlots[i].Y);
		FInventorySlot SlotToCheck = FInventorySlot(CoveredSlotPosition,false);

		int32 Index;

		//Check if slot is found
		if(InventorySlots.Find(SlotToCheck,Index))
		{
			if(InventorySlots[Index].bIsOccupied == true)
			{
				//return false for check if slot is already occupied
				return false;
			}
		}
		else
		{
			//Return false for check if not a valid slot
			return false;
		}
	}

	//Return true for check if all slots are valid and are not occupied
	return true;
	
}



bool UInventoryComponent::FindSlotAtPosition(FInventory2D TargetPosition, FInventorySlot& OutSlot)
{
	const FInventorySlot DummySlot = FInventorySlot(TargetPosition,false);

	int32 Index;
	if(InventorySlots.Find(DummySlot,Index))
	{
		OutSlot = InventorySlots[Index];
		return true;
	}
	else
	{
		return false;
	}
}

bool UInventoryComponent::FindSlotAtPosition(FInventory2D TargetPosition, int32& OutIndex) const
{
	const FInventorySlot DummySlot = FInventorySlot(TargetPosition,false);
	
	int32 SlotIndex;
	if(InventorySlots.Find(DummySlot,SlotIndex))
	{
		OutIndex = SlotIndex;
		return true;
	}
	else
	{
		return false;
	}
}



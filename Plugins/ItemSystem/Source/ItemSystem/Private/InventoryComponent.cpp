// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ItemBase.h"
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

	//Make sure valid item, if not return false
	if(CheckIfItemValid(Item) == false)
	{
		UE_LOG(LogItemSystem,Warning, TEXT("Attempting to add %s to %s inventory with invalid GUID"),
			*Item.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}

	//Make sure enough weight for item, if note return false
	if(CheckIfItemWeightOK(Item) == false)
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s inventory does not have enought wieght capacity for %s item"),
			*GetOwner()->GetName(),*Item.DisplayName.ToString())
		return false;
	}
	
	FInventorySlot TargetSlot;
	const bool bIsValidSlot = FindSlotAtPosition(Position,TargetSlot);

	//Check if the slot is valid
	if(bIsValidSlot == false)
	{
		UE_LOG(LogItemSystem,Warning,TEXT("Attempting to add item %s to invalid slot in %s inventory"),
			*Item.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}

	//If target slot is occupied return false.  No need to log since occupied slots are expected
	if(TargetSlot.bIsOccupied == true)
	{
		return false;
	}
	else
	{
		//See if it will fit in the target slot by checking the slots that would be covered to see if they're occupied
		if(CheckIfItemFits(Item,TargetSlot.Position))
		{
			const FInventoryItemData NewInventoryItem = FInventoryItemData(TargetSlot.Position,Item);

			if(SetSlotStatuses(NewInventoryItem.GetCoveredSlots(),true))
			{
				InventoryItems.Add(NewInventoryItem);
				AddWeight(Item);

				UE_LOG(LogItemSystem,Log,TEXT("Added %s item to position %s in %s inventory"),
					*Item.DisplayName.ToString(),*Position.GetPositionAsString(),
					*GetOwner()->GetName())
				
				return true;
			}
			else
			{
				//Would be false here if there was an issue setting the slot statuses.  
				UE_LOG(LogItemSystem,Error,TEXT("%s could not be added to %s inventory for unknown reason"),
					*Item.DisplayName.ToString(),*GetOwner()->GetName())
				return false;
			}
		}
		else
		{
			//Returns false if the item could not fit at the target slot 
			return false;
		}
	}
}

bool UInventoryComponent::RemoveInventoryItem(FInventoryItemData TargetInventoryItem)
{
	int32 ItemIndex;
	if(InventoryItems.Find(TargetInventoryItem,ItemIndex))
	{
		RemoveWeight(InventoryItems[ItemIndex].Item);
		InventoryItems.RemoveAt(ItemIndex);

		UE_LOG(LogItemSystem,Log,TEXT("Removed %s item from position %s in %s inventory"),
			*TargetInventoryItem.Item.DisplayName.ToString(),*TargetInventoryItem.StartPosition.GetPositionAsString(),
			*GetOwner()->GetName());
		
		return true;
		
	}
	else
	{
		UE_LOG(LogItemSystem, Warning, TEXT("Attempted to remove %s item from %s inventory but could not match GUID"),
			*TargetInventoryItem.Item.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}
}

bool UInventoryComponent::FindInventoryItemAtPosition(const FInventory2D Position, FInventoryItemData& OutInventoryItemData)
{
	for (int i = 0; i < InventoryItems.Num(); ++i)
	{
		FInventoryItemData TargetInventoryItem = InventoryItems[i];

		if(TargetInventoryItem.StartPosition == Position)
		{
			OutInventoryItemData = TargetInventoryItem;
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::SetSlotStatus(const FInventory2D TargetPosition, const bool NewIsOccupied)
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

bool UInventoryComponent::CheckIfItemValid(const FItemData ItemData)
{
	
	return ItemData.ItemGuid.IsValid() && ItemData.InWorldClass != nullptr;
}


bool UInventoryComponent::CheckIfItemWeightOK(const FItemData ItemData) const
{
	return (CurrentWeight + ItemData.GetStackWeight() <= MaxWeight);
}

void UInventoryComponent::AddWeight(const FItemData ItemData)
{
	const float WeightToAdd = ItemData.GetStackWeight();
	CurrentWeight = FMath::Clamp(CurrentWeight + WeightToAdd,0.f,MaxWeight);
}

void UInventoryComponent::RemoveWeight(const FItemData ItemData)
{
	const float WeightToRemove = ItemData.GetStackWeight();
	CurrentWeight = FMath::Clamp(CurrentWeight - WeightToRemove,0.f,MaxWeight);
}





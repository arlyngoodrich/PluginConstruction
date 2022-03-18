// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ItemBase.h"
#include "ItemSystem.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	InventoryName = FText::FromString("Inventory");
	InventorySize = FInventory2D(2,2);
	MaxWeight = 10.f;
	
}

int32 UInventoryComponent::GetSlotCount() const { return InventorySlots.Num(); }
int32 UInventoryComponent::GetItemCount() const{return InventoryItems.Num();}
float UInventoryComponent::GetInventoryWeight() const { return CurrentWeight; }
float UInventoryComponent::GetInventoryMaxWeight() const { return MaxWeight; }

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventorySlots);
	DOREPLIFETIME(UInventoryComponent, InventoryItems);
	DOREPLIFETIME(UInventoryComponent, CurrentWeight);
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeSlots();

	// ...
	
}

void UInventoryComponent::OnRegister()
{
	Super::OnRegister();
	InitializeSlots();
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

	bHaveSlotsBeenInitialized = true;
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

	if(false == AddItemChecks(Item)) {return false;}
	
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

bool UInventoryComponent::AutoAddItem(const FItemData Item)
{
	if(AddItemChecks(Item) == false){return false;}

	//Cycle through slots and attempt to add items
	for (int i = 0; i < InventorySlots.Num(); ++i)
	{
		const FInventorySlot TargetSlot = InventorySlots[i];

		if(TargetSlot.bIsOccupied == false)
		{
			if(AddItemToPosition(Item,TargetSlot.Position))
			{
				return true;
			}
		}
	}

	UE_LOG(LogItemSystem,Log,TEXT("%s item can not fit into %s inventory"),
		*Item.DisplayName.ToString(),*GetOwner()->GetName())
	return false;
	
}

bool UInventoryComponent::AutoAddItem(const FItemData InItem, FItemData& OutRemainingItem)
{
	if(AddItemChecks(InItem) == false) {return false;}
	OutRemainingItem = InItem;

	//Try to stack into existing stack
	if(InItem.bShouldItemStack == true)
	{
		for (int i = 0; i < InventoryItems.Num(); ++i)
		{
			const FInventoryItemData TargetInventoryItem = InventoryItems[i];

			//Check to see if they are the same class 
			if(InItem.InWorldClass->StaticClass() == TargetInventoryItem.Item.InWorldClass->StaticClass())
			{
				if(AttemptStack(TargetInventoryItem,InItem,OutRemainingItem))
				{
					//Item Fully stacked
					return true;
				}
			}
		}
	}

	//Stack Remaining Quantity
	if(AutoAddItem(OutRemainingItem))
	{
		//New Stack created and added
		OutRemainingItem = FItemData();
		return true;
	}
	else
	{
		//Not enough room to add new stack 
		return false;
	}
	
}

bool UInventoryComponent::ReduceQuantityOfItemByStaticClass(const TSubclassOf<AItemBase> ItemClass, int32 QuantityToRemove,
                                                            int32& OutAmountNotRemoved)
{
	
	//Cycle through all items in the inventory
	for (int i = 0; i < InventoryItems.Num(); ++i)
	{
		const FInventoryItemData TargetInventoryItemData = InventoryItems[i];

		//Check to see if the class matches
		if(TargetInventoryItemData.Item.InWorldClass->StaticClass() == ItemClass->StaticClass())
		{
			
			//If it does, attempt to remove items
			if(ReduceQuantityOfInventoryItem(TargetInventoryItemData,QuantityToRemove,OutAmountNotRemoved))
			{
				if(OutAmountNotRemoved == 0)
				{
					return true;
				}
				else
				{
					QuantityToRemove = OutAmountNotRemoved;
				}
			}
		}
	}

	// If at least some amount was removed, then return true
	//Return false if nothing was removed
	return (OutAmountNotRemoved > 0);
}



bool UInventoryComponent::ReduceQuantityOfInventoryItem(const FInventoryItemData TargetInventoryItem,
                                                        const int32 QuantityToRemove,
                                                        int32& OutAmountNotRemoved)
{
	int32 ItemIndex;

	OutAmountNotRemoved = QuantityToRemove;
	
	if(InventoryItems.Find(TargetInventoryItem,ItemIndex))
	{
		const int32 ItemQuantity = InventoryItems[ItemIndex].Item.ItemQuantity;

		//Check to see if actually partially removing
		if(ItemQuantity > QuantityToRemove)
		{
			const int32 QuantityRemaining = ItemQuantity  - QuantityToRemove;
			InventoryItems[ItemIndex].Item.ItemQuantity = QuantityRemaining;

			const float PerItemWeight = InventoryItems[ItemIndex].Item.PerItemWeight;
			RemoveWeight(PerItemWeight*QuantityToRemove);
			OutAmountNotRemoved = 0.f;

			UE_LOG(LogItemSystem,Log,TEXT("%s item had %d quantity removed in %s inventory"),
				*TargetInventoryItem.Item.DisplayName.ToString(),QuantityToRemove,*GetOwner()->GetName())
			
			return true;
			
		}
		else
		//If quantity to remove is >= item quantity
		//Remove the full stack of the item
		{
			if(FullyRemoveInventoryItem(TargetInventoryItem))
			{
				OutAmountNotRemoved = QuantityToRemove - ItemQuantity;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		UE_LOG(LogItemSystem,Log,TEXT("Could not find %s item in %s inventory to remove"),
	*TargetInventoryItem.Item.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}
}

bool UInventoryComponent::FullyRemoveInventoryItem(const FInventoryItemData TargetInventoryItem)
{
	int32 ItemIndex;
	if(InventoryItems.Find(TargetInventoryItem,ItemIndex))
	{

		//Uncover Slots
		if(SetSlotStatuses(TargetInventoryItem.GetCoveredSlots(),false))
		{
			InventoryItems.RemoveAt(ItemIndex);
			RemoveWeight(TargetInventoryItem.Item);

			UE_LOG(LogItemSystem,Log,TEXT("%s item was fully removed from %s inventory"),
				*TargetInventoryItem.Item.DisplayName.ToString(),*GetOwner()->GetName())

			return true;
		}
		else
		{
			UE_LOG(LogItemSystem,Log,
				TEXT("%s item could not removed from %s inventory because of issue uncovering the slots"),
				*TargetInventoryItem.Item.DisplayName.ToString(),*GetOwner()->GetName())
			return false;
		}
		
	}
	else
	{
		UE_LOG(LogItemSystem,Log,TEXT("Could not find %s item in %s inventory to remove"),
			*TargetInventoryItem.Item.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}
}

bool UInventoryComponent::IsItemInInventory(const FItemData Item)
{
	for (int i = 0; i < InventoryItems.Num(); ++i)
	{
		FInventoryItemData InventoryItemData = InventoryItems[i];
		if(InventoryItemData.Item == Item)
		{
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::IsItemInInventory(const FItemData Item, FInventory2D& OutItemPosition)
{
	for (int i = 0; i < InventoryItems.Num(); ++i)
	{
		FInventoryItemData InventoryItemData = InventoryItems[i];
		if(InventoryItemData.Item == Item)
		{
			OutItemPosition = InventoryItemData.StartPosition;
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::AttemptStack(FInventoryItemData TargetItemData, FItemData InItemData,
                                       FItemData& OutRemainingItem)
{
	OutRemainingItem = InItemData;

	if(AddItemChecks(InItemData)==false)
	{
		return false;
	}

	if(TargetItemData.Item.bShouldItemStack == false)
	{
		UE_LOG(LogItemSystem,Log,TEXT("Cannot stack %s item in %s inventory"),
		       *InItemData.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}

	int32 TargetItemIndex;
	if(InventoryItems.Find(TargetItemData,TargetItemIndex) == false)
	{
		UE_LOG(LogItemSystem,Log,TEXT("Attempting to stack item %s that is not in %s inventory"),
		       *InItemData.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}

	if(TargetItemData.Item.MaxStackQuantity == TargetItemData.Item.ItemQuantity)
	{
		//return false since target stack is already.  This is expected to happen so no need to log. 
		return false;
	}
	
	const int32 TargetStackQty = TargetItemData.Item.ItemQuantity;
	const int32 TargetMaxQty = TargetItemData.Item.MaxStackQuantity;
	const int32 TargetCapacity = TargetMaxQty - TargetStackQty;
	const int32 IncomingStackQty = InItemData.ItemQuantity;

	if(TargetCapacity >= IncomingStackQty)
	{
		//Full Stack Item
		InventoryItems[TargetItemIndex].Item.ItemQuantity += IncomingStackQty;
		AddWeight(InItemData);

		//Invalidate Remaining Item
		OutRemainingItem = FItemData();

		UE_LOG(LogItemSystem,Log,TEXT("%s was fully stacked into slot %s of %s inventory"),
		       *InItemData.DisplayName.ToString(),*InventoryItems[TargetItemIndex].StartPosition.GetPositionAsString(),
		       *GetOwner()->GetName())
		
		return true;
	}
	else
	{
		//Partially stack item
		//Set Target stack to max amount
		InventoryItems[TargetItemIndex].Item.ItemQuantity = TargetMaxQty;
		AddWeight(InItemData.PerItemWeight * TargetCapacity);

		OutRemainingItem.ItemQuantity -= TargetCapacity;

		UE_LOG(LogItemSystem,Log,TEXT("%s was partially stacked into slot %s of %s inventory"),
		       *InItemData.DisplayName.ToString(),*InventoryItems[TargetItemIndex].StartPosition.GetPositionAsString(),
		       *GetOwner()->GetName())
		
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
			*GetOwner()->GetName(),*TargetPosition.GetPositionAsString(),
			NewIsOccupied? TEXT("occupied") : TEXT("unoccupied"))
		return true;
	}
	else
	{
		UE_LOG(LogItemSystem,Warning,TEXT("Could not find %s in %s inventory to set status"),
			*TargetPosition.GetPositionAsString(),*GetOwner()->GetName())
		return false;
	}
}

bool UInventoryComponent::SetSlotStatuses(TArray<FInventory2D> TargetPositions, const bool NewIsOccupied)
{

	TArray<bool> SlotChecks;
	
	for (int i = 0; i < TargetPositions.Num(); ++i)
	{
		bool SlotCheck = SetSlotStatus(TargetPositions[i], NewIsOccupied);
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

bool UInventoryComponent::AddItemChecks(const FItemData ItemToCheck) const
{
	//Make sure valid item, if not return false
	if(CheckIfItemValid(ItemToCheck) == false)
	{
		UE_LOG(LogItemSystem,Warning, TEXT("Attempting to add %s to %s inventory with invalid GUID"),
			*ItemToCheck.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}

	//Make sure enough weight for item, if note return false
	if(CheckIfItemWeightOK(ItemToCheck) == false)
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s inventory does not have enought wieght capacity for %s item"),
			*GetOwner()->GetName(),*ItemToCheck.DisplayName.ToString())
		return false;
	}

	return true;
}

void UInventoryComponent::AddWeight(const FItemData ItemData)
{
	const float WeightToAdd = ItemData.GetStackWeight();
	CurrentWeight = FMath::Clamp(CurrentWeight + WeightToAdd,0.f,MaxWeight);

	UE_LOG(LogItemSystem,Log,TEXT("Added %s weight, new weight %s in %s inventory"),
*FString::SanitizeFloat(WeightToAdd),*FString::SanitizeFloat(CurrentWeight),*GetOwner()->GetName());
}

void UInventoryComponent::AddWeight(const float AddWeight)
{
	CurrentWeight = FMath::Clamp(CurrentWeight + AddWeight,0.f,MaxWeight);
	
	UE_LOG(LogItemSystem,Log,TEXT("Added %s weight, new weight %s in %s inventory"),
*FString::SanitizeFloat(AddWeight),*FString::SanitizeFloat(CurrentWeight),*GetOwner()->GetName());
}

void UInventoryComponent::RemoveWeight(const FItemData ItemData)
{
	const float WeightToRemove = ItemData.GetStackWeight();
	CurrentWeight = FMath::Clamp(CurrentWeight - WeightToRemove,0.f,MaxWeight);

	UE_LOG(LogItemSystem,Log,TEXT("Removed %s weight, new weight %s in %s inventory"),
	*FString::SanitizeFloat(WeightToRemove),*FString::SanitizeFloat(CurrentWeight),*GetOwner()->GetName());
}

void UInventoryComponent::RemoveWeight(const float RemoveWeight)
{
	CurrentWeight = FMath::Clamp(CurrentWeight - RemoveWeight,0.f,MaxWeight);

	UE_LOG(LogItemSystem,Log,TEXT("Removed %s weight, new weight %s in %s inventory"),
		*FString::SanitizeFloat(RemoveWeight),*FString::SanitizeFloat(CurrentWeight),*GetOwner()->GetName());
}





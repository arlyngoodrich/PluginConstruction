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

	
	
}


bool UInventoryComponent::CheckIfItemFits(FInventoryItemData InventoryItemData, const FInventory2D TargetPosition)
{
	
	InventoryItemData.StartPosition = TargetPosition;
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


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryData.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent) )
class ITEMSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//UI display name of the inventory.  Cosmetic only. 
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Inventory Data")
	FText InventoryName;

	//2D size of the inventory
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Invetory Data")
	FInventory2D InventorySize;

	//Max weight that the inventory can hold 
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Inventory Data")
	float MaxWeight;

	//Current weight of the inventory
	UPROPERTY(BlueprintReadOnly,Category="Inventory Data")
	float CurrentWeight;

	//Array of slots that can hold items in the inventory
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory Data")
	TArray<FInventorySlot> InventorySlots;

	//Array of items and their positions in the inventory
	UPROPERTY(Replicated, BlueprintReadOnly,Category="Inventory Data");
	TArray<FInventoryItemData> InventoryItems;

	//Utilitizes Inventory Size to reset array of InventorySlots.  All slots will be set to unoccupied.  
	void InitializeSlots();

	//Method to initialize inventory from an existing InventoryItemData array.   
	void SetInventory(TArray<FInventoryItemData> InInventoryItems);

	//Checks if the item will fit into a given position by check slots that would be covered by the item.  Returns false
	//if it will not fit and true if it will. 
	bool CheckIfItemFits(FItemData ItemData, FInventory2D TargetPosition);

	//Adds and item to a position.  Will return false if: Position not found, item is not valid, weight cannot be added
	bool AddItemToPosition(FItemData Item, FInventory2D Position);

	//Checks for Item's GUID and removes it.  Does not invalidate item -- just removes item and weight from inventory.
	//Will return false if Item GUID not found in inventory. 
	bool RemoveInventoryItem(FInventoryItemData TargetInventoryItem);
	
	//Given a position, will return the item in that position.  True if an item is found and false if no item is found.
	bool FindInventoryItemAtPosition(FInventory2D Position, FInventoryItemData& OutInventoryItemData);

	//Given a positions, will update the slot statuses to the NewIsOccupied.  Will return false if the position
	//could not be found in the InventorySlot array.
	bool SetSlotStatus(FInventory2D TargetPosition, bool NewIsOccupied);

	//Given an array of positions, will update the slot statuses to the NewIsOccupied.  Will return false if a position
	//could not be found in the InventorySlot array.
	bool SetSlotStatuses(TArray<FInventory2D> TargetPositions, bool NewIsOccupied);

	//Given a target position, will return the InventorySlot reference from the InventorySlot Array. Will return false
	//if target position is not found in the array.
	bool FindSlotAtPosition(FInventory2D TargetPosition, FInventorySlot& OutSlot);

	//Given a target position, will return the index of that position from the InventorySlot Array. Will return false
	//if target position is not found in the array.
	bool FindSlotAtPosition(FInventory2D TargetPosition, int32& OutIndex) const;

	//Ensures GUID is valid and the In World Class is not a nullptr.
	static bool CheckIfItemValid(FItemData ItemData);

	//Returns true if the Item can be added to the current weight without exceeding MaxWeight.
	bool CheckIfItemWeightOK(FItemData ItemData) const;

	//Adds the weight of Item's stack to the current weight.  Clamped between 0 and MaxWeight.
	void AddWeight(FItemData ItemData);

	//Removes the weight of Item's stack from the current weight.  Clamped between 0 and MaxWeight.
	void RemoveWeight(FItemData ItemData);
	
	
};

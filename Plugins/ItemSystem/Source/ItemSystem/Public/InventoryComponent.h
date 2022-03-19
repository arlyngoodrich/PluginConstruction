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

	//Returns the number of slots in the inventory
	int32 GetSlotCount() const;

	//Returns the number of items in the inventory
	int32 GetItemCount() const;
	
	//Returns the current weight of the inventory
	float GetInventoryWeight() const;

	//Returns the max weight of the inventory
	float GetInventoryMaxWeight() const;

	//Returns the quantity of items per class
	int32 GetTotalCountOfItemClass(TSubclassOf<AItemBase> ItemClass);
	
	//Adds and item to a position.  Will return false if target position not found, item is not valid, weight cannot be
	//added
	bool AddItemToPosition(FItemData Item, FInventory2D Position);

	//Attempts to transfer target item from Instigating Inventory (Inventory calling the method).  Will return true if
	//item was fully or partially transferred. Will return false if the item was not transferred or if there was an error.
	bool TransferItem(UInventoryComponent* TargetInventory,FInventoryItemData TargetItem);

	//Attempts to transfer target item from Instigating Inventory (Inventory calling the method) to a specific position
	//in the target inventory.  Will return true if the the item was fully OR partially transferred.  Will return false
	//if the item was not transferred or if there was an error.  
	bool TransferItemToPosition(UInventoryComponent* TargetInventory, FInventory2D TargetPosition,
	                            FInventoryItemData TargetItem);

	//Will attempt to add item into existing stacks.  If it cannot add to an existing stack, will attempt to add as a
	//new stack. Will return remaining amount of item.
	//Returns true if stack was full added and returns false if was partially added. 
	bool AutoAddItem(FItemData InItem, FItemData& OutRemainingItem);

	//Will attempt to add item into existing stacks.  If it cannot add to an existing stack, will attempt to add as a
	//new stack.  Will NOT return remaining item.
	//Returns true if was fully or partially stacked.  Returns false if could not be stacked at all. 
	bool AutoAddItem(FItemData InItem);

	//Cycles through items in inventory that matches the item's exact class and will remove as much of the quantity to
	//remove as possible.
	//Will return false if a matching item class could not be found in the inventory
	bool ReduceQuantityOfItemByStaticClass(TSubclassOf<AItemBase> ItemClass, int32 QuantityToRemove,
	                                       int32& OutAmountNotRemoved);
	
	//Checks for Items by GUID and Position and reduces it's item quantity.
	//Will fully remove item if the removal amount is greater than or equal to the current amount.  Will return the amount
	//that was not removed
	//Will return false if the Item GUID and position not found in inventory
	bool ReduceQuantityOfInventoryItem(FInventoryItemData TargetInventoryItem, int32 QuantityToRemove,
									   int32& OutAmountNotRemoved);

	//Checks for Items by GUID and Position and reduces it's item quantity.
	//Will fully remove item if the removal amount is greater than or equal to the current amount.  Will NOT return the
	//Amount that was removed
	//Will return false if the Item GUID and position not found in inventory OR false if the item was not fully removed
	bool ReduceQuantityOfInventoryItem(FInventoryItemData TargetInventoryItem, int32 QuantityToRemove);
	
	//Checks for Items by GUID and Position and removes it.
	//Does not invalidate item -- just removes item and weight from inventory.
	//Will return false if Item GUID and position not found in inventory. 
	bool FullyRemoveInventoryItem(FInventoryItemData TargetInventoryItem);


	//Moves item to a new position in inventory.  Returns true if the item is moved, returns false if not.  
	bool MoveItem(FInventoryItemData TargetItem, FInventory2D TargetPosition, bool bRotateITem);

	
	//Checks to see if Item is in Inventory.  Checks for matching Item GUIDs. Returns true if found
	bool IsItemInInventory(FItemData Item);

	//Checks to see if Item is in Inventory by checking for matching Item GUIDs. Returns true if found and the position
	//of the item in the inventory.
	bool IsItemInInventory(FItemData Item, FInventory2D& OutItemPosition);

	//Checks to see if Item is in Inventory by checking for matching Item GUIDs. Returns true if found and the InventoryItemData
	//of the item in the inventory.
	bool IsItemInInventory(FItemData Item, FInventoryItemData& OutInventoryItemData);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Called when Inventory is created dynamically
	virtual void OnRegister() override;
	
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
	UPROPERTY(Replicated, BlueprintReadOnly,Category="Inventory Data")
	float CurrentWeight;

	//Array of slots that can hold items in the inventory
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory Data")
	TArray<FInventorySlot> InventorySlots;

	//Array of items and their positions in the inventory
	UPROPERTY(Replicated, BlueprintReadOnly,Category="Inventory Data");
	TArray<FInventoryItemData> InventoryItems;

	//Set to true when slots have been created. 
	UPROPERTY(BlueprintReadOnly, Category="Inventory Data")
	bool bHaveSlotsBeenInitialized; 
	
	//Utilitizes Inventory Size to reset array of InventorySlots.  All slots will be set to unoccupied.  
	void InitializeSlots();
	
	//Method to initialize inventory from an existing InventoryItemData array.   
	void SetInventory(TArray<FInventoryItemData> InInventoryItems);

	//Checks if the item will fit into a given position by check slots that would be covered by the item.  Returns false
	//if it will not fit and true if it will. 
	bool CheckIfItemFits(FItemData ItemData, FInventory2D TargetPosition);

	//Cylces through all slots until item is added.  Will return false if: Item is not valid, weight cannot be added,
	//or Item cannot fit in any positions
	bool AutoAddItemNewStack(FItemData Item);
	
	//Attempts to stack given Item Data into existing stack.  Will return true if fully stacked, will return false if not.
	bool AttemptStack(FInventoryItemData TargetItemData, FItemData InItemData, FItemData& OutRemainingItem);
	
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

	//Helper function that checks if the item is valid and that it's weight can be added
	bool AddItemChecks(FItemData ItemToCheck) const;

	//Helper functions that performs checks before transferring items
	bool TransferItemChecks(FInventoryItemData ItemToCheck,UInventoryComponent* InventoryToCheck) const;

	//Adds the weight of Item's stack to the current weight.  Clamped between 0 and MaxWeight.
	void AddWeight(FItemData ItemData);

	//Adds a specific amount of weight to the current weight.  Use items getting stacked when added
	void AddWeight(float AddWeight);

	//Removes the weight of Item's stack from the current weight.  Clamped between 0 and MaxWeight.
	void RemoveWeight(FItemData ItemData);

	void RemoveWeight(float RemoveWeight);
	
};

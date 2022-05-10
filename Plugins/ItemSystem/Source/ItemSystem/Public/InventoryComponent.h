// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryData.h"
#include "InventoryComponent.generated.h"

//Called when updates are made to the items in the inventory.  Will need to get inventory item data directly. 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryItemDataUpdate);
//Called when updates are made to the slots in the inventory. Will need to get slot data directly.  
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventorySlotUpdate);


/**
 * @brief Actor component parent class for inventories 
 */
UCLASS( ClassGroup=(Inventory), Blueprintable, meta=(BlueprintSpawnableComponent) )
class ITEMSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Sets default values for this component's properties
	 */
	UInventoryComponent();
	
	/**
	 * @brief Returns the number of slots in the inventory
	 */
	int32 GetSlotCount() const;
	
	/**
	 * @brief Returns the number of items in the inventory
	 */
	int32 GetItemCount() const;
	
	/**
	 * @brief Returns the current weight of the inventory
	 */
	float GetInventoryWeight() const;
	
	/**
	 * @brief Returns the max weight of the inventory
	 */
	float GetInventoryMaxWeight() const;
	
	/**
	 * @brief Returns the inventory items 
	 */
	TArray<FInventoryItemData> GetInventoryItemData () const;
	
	/**
	 * @brief Returns Inventories Slots
	 */
	TArray<FInventorySlot> GetInventorySlots() const;
	
	/**
	 * @brief Returns the quantity of items per exact class
	 * @param ItemClass class of item to find quantity of
	 * @return Summation of all item stacks that match the item class
	 */
	int32 GetTotalCountOfItemClass(TSubclassOf<AItemBase> ItemClass);

	/**
	 * @brief Returns the quantity of items of a class type and sub types
	 * @param ItemClass class of item to find quantity of
	 * @return Summation of all item stacks that match the item class
	 */
	int32 GetTotalCountOfItemSubClass(TSubclassOf<AItemBase> ItemClass);
	

	/**
	* @brief Adds and item to a position.  Public for debugging and testing.  Should not be directly used.   
	 * @param Item Item to add to position
	 * @param Position Target position that item will be added
	 * @return Will return false if target position not found, item is not valid, weight cannot be added
	 */
	bool AddItemToPosition(FItemData Item, FInventory2D Position);


	/**
	 * @brief Attempts to transfer target item from Instigating Inventory (Inventory calling the method). UI Accessible Function.
	 * @param TargetInventory Inventory that item will be transferred to
	 * @param TargetItem  Item that will be transferred
	 * @return Will return true if item was fully or partially transferred. Will return false if the item was not transferred
	 * or if there was an error.
	 */
	bool TransferItem(UInventoryComponent* TargetInventory,FInventoryItemData TargetItem);
	
	/**
	* @brief Attempts to transfer target item from Instigating Inventory (Inventory calling the method) to a specific position
	* in the target inventory. UI Accessible Function.
	 * @param TargetInventory Inventory the item will be transferred to
	 * @param TargetPosition Position in target inventory the item will be transferred to
	 * @param TargetItem Item that will transfer
	* @return Will return true if the the item was fully OR partially transferred.  Will return false
	* if the item was not transferred or if there was an error. 
	 */
	bool TransferItemToPosition(UInventoryComponent* TargetInventory, FInventory2D TargetPosition,
	                            FInventoryItemData TargetItem);


	/**
	* @brief Attempts to transfer target item from Instigating Inventory (Inventory calling the method) to a specific position
	* in the target inventory.  UI Accessible Function.
	 * @param TargetInventory Inventory the item will be transferred to
	 * @param TargetPosition Position in target inventory the item will be transferred to
	 * @param TargetItem Item that will transfer
	 * @param bRotateItem If the item should be rotated or not
	* @return    Will return true if the the item was fully OR partially transferred.  Will return false
	if the item was not transferred or if there was an error.  Will attempt RPC if not authority.
	 */
	bool TransferItemToPosition(UInventoryComponent* TargetInventory, FInventory2D TargetPosition,
								FInventoryItemData TargetItem, bool bRotateItem);

	
	/**
	* @brief Will attempt to add item into existing stacks.  If it cannot add to an existing stack, will attempt to add as a
	* new stack.
	 * @param InItem Originating item
	 * @param OutRemainingItem Remaining item.  Will be null if item was fully added.
	 * @return  Will return remaining amount of item. Returns true if stack was full added and returns false if was partially added.
	 */
	bool AutoAddItem(FItemData InItem, FItemData& OutRemainingItem);
	
	/**
	* @brief Will attempt to add item into existing stacks.  If it cannot add to an existing stack, will attempt to add as a
	* new stack. Does not return leftover item amount.
	 * @param InItem Originating item
	 * @return Returns true if stack was full added and returns false if was partially added.
	 */
	bool AutoAddItem(FItemData InItem);

	/**
	* @brief Splits the target item stack into a new unique item stack.  Will cycle new item through slots until it's added.
	* Will attempt RPC if not authority. UI Accessible Function
	* @param TargetItemData Item data that will be split
	* @param NewStackQuantity Quantity that should be split from the original stack into a new stack
	* @return Returns false if the new stack quantity is greater than what is in the existing stack, if the new stack cannot go
	* to the target position, or if the target item does not exist.  Will return true if the stack is successfully split.
	*/
	bool SplitItem(FInventoryItemData TargetItemData, int32 NewStackQuantity);


	/**
	 * @brief Splits the target item stack into a new unique item stack at the target position.
	 * @param TargetItemData Item data that will be split
	 * @param TargetPosition Position that new item stack will go to
	 * @param NewStackQuantity  Quantity that should be split from the original stack into the new stack
	 * @return Will return false if the new stack quantity is greater than what is in the existing stack, if the new
	 * stack cannot go to the target position, or if the target item does not exist.  Will return true if the stack is
	 * successfully split.
	 */
	bool SplitItemStackToPosition(FInventoryItemData TargetItemData, FInventory2D TargetPosition, int32 NewStackQuantity);


	/**
	* @brief Cycles through items in inventory that matches the item's exact class and will remove as much of the quantity to
	* remove as possible.
	 * @param ItemClass Item class targeted for quantity removal
	 * @param QuantityToRemove Amount of item that should be removed
	 * @param OutAmountNotRemoved Quantity requested removed minus amount actually removed
	 * @return Will return false if not authority or a matching item class could not be found in the inventory
	 */
	bool ReduceQuantityOfItemByStaticClass(TSubclassOf<AItemBase> ItemClass, int32 QuantityToRemove,
	                                       int32& OutAmountNotRemoved);
	

	/**
	* @brief Cycles through items in inventory that matches the item's class or sub type and will remove as much of the
	* quantity to remove as possible.
	 * @param ItemClass Item class targeted for quantity removal
	 * @param QuantityToRemove Amount of item that should be removed
	 * @param OutAmountNotRemoved Quantity requested removed minus amount actually removed
	 * @return Will return false if not authority or a matching item class could not be found in the inventory
	 */
	bool ReduceQuantityOfItemByClassSubType(TSubclassOf<AItemBase> ItemClass, int32 QuantityToRemove,
										   int32& OutAmountNotRemoved);
	
	/**
	 * @brief Checks for Items by GUID and Position and reduces it's item quantity. Will fully remove item if the removal amount
	 * is greater than or equal to the current amount.  Will return the amount that was not removed.
	 * @param TargetInventoryItem Target item to remove quantity from
	 * @param QuantityToRemove Request amount that should be removed from target item
	 * @param OutAmountNotRemoved AQuantity request removed minus amount actually removed
	 * @return  Will return false if the Item GUID and position not found in inventory
	 */
	bool ReduceQuantityOfInventoryItem(FInventoryItemData TargetInventoryItem, int32 QuantityToRemove,
									   int32& OutAmountNotRemoved);
	
	/**
	 * @brief Checks for Items by GUID and Position and reduces it's item quantity.
	 * Will fully remove item if the removal amount is greater than or equal to the current amount.  Will NOT return the
	 * Amount that was removed.  Does not return a leftover amount.
	 * @param TargetInventoryItem Target item to remove quantity from
	 * @param QuantityToRemove Quantity to remove from target item
	 * @return Will return false if the Item GUID and position not found in inventory OR false if the item was not fully removed
	 */
	bool ReduceQuantityOfInventoryItem(FInventoryItemData TargetInventoryItem, int32 QuantityToRemove);
	
	/**
	 * @brief Checks for Items by GUID and Position and removes it. Does not invalidate item -- just removes item and weight
	 * from inventory.
	 * @param TargetInventoryItem item that should be fully removed. 
	 * @return Will return false if Item GUID and position not found in inventory. 
	 */
	bool FullyRemoveInventoryItem(FInventoryItemData TargetInventoryItem);

	//
	/**
	 * @brief Performs same checks as moving the item but does not move it.  Useful for UI/client checks. 
	 * @param TargetItem Item to see if it could be moved
	 * @param TargetPosition Position to see if item can move to
	 * @param bRotateItem If the item would be rotated or not during the move
	 * @return True if Item could move to target position and false if it could not. 
	 */
	bool CheckItemMove(FInventoryItemData TargetItem, FInventory2D TargetPosition, bool bRotateItem);
	

	/**
	 * @brief Moves item to a new position in inventory. Will attempt RPC if not authority. UI Accessible Function 
	 * @param TargetItem Item that will be moved
	 * @param TargetPosition Position that item will be moved to
	 * @param bRotateITem If item should be rotated or not when moved
	 * @return Returns true if the item is moved, returns false if not.
	 */
	bool MoveItem(FInventoryItemData TargetItem, FInventory2D TargetPosition, bool bRotateITem);


	/**
	 * @brief Combines two items stacks into one.  If fully combined, will remove Originating stack. If partially combined,
	 * will update originating stack. 
	 * @param OriginatingStack Stack that is being combined into another stack
	 * @param TargetStack Stack that is the originating stack is combining into
	 * @return returns false if stacks are not of the same class or if target stack is already max.  True if at least 1 quantity
	 * added to target stack.
	 */
	bool CombineStacks_SameInventory(FInventoryItemData OriginatingStack, FInventoryItemData TargetStack);


	/**
	 * @brief Client friendly method that performs same checks as actual combining stacks method.  Helpful for UI drag
	 * and drop operation. 
	 * @param OriginatingStack Item stack instigating the combine operation
	 * @param TargetStack  Item stack receiving the originating stack
	 * @return True if OK to stack fully or partially, false if not
	 */
	bool CombineStacks_SameInventory_Checks(FInventoryItemData OriginatingStack,FInventoryItemData TargetStack) const;


	/**
	* @brief Client friendly method that performs same checks as actual combining stacks method.  Helpful for UI drag
	 * and drop operation.
	 * @param OriginatingStack Item stack instigating the combine operation
	 * @param TargetStack  Item stack receiving the originating stack
	 * @param bOutWillFullyStack True if will fully stack into new item, false if not.  
	 * @return True if OK to stack fully or partially, false if not
	 */
	bool CombineStacks_SameInventory_Checks(FInventoryItemData OriginatingStack,FInventoryItemData TargetStack, bool& bOutWillFullyStack) const;
	
	/**
	 * @brief Checks to see if Item is in Inventory.  Checks for matching Item GUIDs. Useful if position of item in
	 * inventory is unknown
	 * @param Item Item to check for in inventory
	 * @return Returns true if found
	 */
	bool IsItemInInventory(FItemData Item);
	
	/**
	 * @brief Checks to see if Item is in Inventory by checking for matching Item GUIDs. UI accessible function. 
	 * @param Item Item to look for 
	 * @param OutItemPosition Position of item in inventory
	 * @return Returns true if found and the position of the item in the inventory.
	 */
	bool IsItemInInventory(FItemData Item, FInventory2D& OutItemPosition);
	
	/**
	 * @brief  Checks to see if Item is in Inventory by checking for matching Item GUIDs. 
	 * @param Item Item to look for
	 * @param OutInventoryItemData Inventory Item data of item in inventory if found
	 * @return Returns true if found and the InventoryItemData of the item in the inventory.
	 */
	bool IsItemInInventory(FItemData Item, FInventoryItemData& OutInventoryItemData);


	/**
	 * @brief Checks to see if inventory item is in inventory.  
	 * @param InventoryItemData Inventory Item to check 
	 * @return True if in inventory, false if not
	 */
	bool IsInventoryItemInInventory(FInventoryItemData InventoryItemData) const;
	
	/**
	 * @brief Given a position, will return the item in that position. 
	 * @param Position Position to get item from
	 * @param OutInventoryItemData Inventory item data in target position
	 * @return True if an item is found and false if no item is found.
	 */
	bool FindInventoryItemAtPosition(FInventory2D Position, FInventoryItemData& OutInventoryItemData);


	/**
	 * @brief Drops item into world.  Checks for free spot in a semi-circle in front of owner to spawn item.  Will then
	 * enable physics timer so item can drop. 
	 * @param ItemData Inventory item to drop
	 */
	void DropItem(FInventoryItemData ItemData);
	
	/**
	 * @brief Checks if the item will fit into a given position by checking slots that would be covered by the item. 
	 * @param ItemData Item to check
	 * @param TargetPosition Target position to check
	 * @return Returns false if it will not fit and true if it will. 
	 */
	bool CheckIfItemFits(FItemData ItemData, FInventory2D TargetPosition);

	/**
	 * @brief Delegate called when inventory is updated.  Used for UI updates.  
	 */
	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FInventoryItemDataUpdate OnInventoryUpdate;
	
	/**
	 * @brief Delegated called when inventory slots are updated.  Used for UI updates. 
	 */
	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FInventorySlotUpdate OnInventorySlotUpdate;

	
protected:
	/**
	 * @brief  Called when the game starts
	 */
	virtual void BeginPlay() override;
	
	/**
	 * @brief Called when Inventory is created dynamically
	 */
	virtual void OnRegister() override;
	
	/**
	 * @brief UI display name of the inventory.  Cosmetic only. 
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Inventory Data")
	FText InventoryName;
	
	/**
	 * @brief 2D size of the inventory
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Invetory Data")
	FInventory2D InventorySize;
	
	/**
	 * @brief Max weight that the inventory can hold 
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Inventory Data")
	float MaxWeight;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Inventory Data")
	float DropPointMinDistance = 200.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Inventory Data")
	float DropPointRadius = 50.f;
	
	/**
	 * @brief Current weight of the inventory
	 */
	UPROPERTY(Replicated, BlueprintReadOnly,Category="Inventory Data")
	float CurrentWeight;
	
	/**
	 * @brief Array of slots that can hold items in the inventory
	 */
	UPROPERTY(ReplicatedUsing=OnRep_InventorySlotsUpdated, BlueprintReadOnly, Category="Inventory Data")
	TArray<FInventorySlot> InventorySlots;
	
	/**
	 * @brief Array of items and their positions in the inventory
	 */
	UPROPERTY(ReplicatedUsing = OnRep_InventoryItemsUpdated, BlueprintReadOnly,Category="Inventory Data")
	TArray<FInventoryItemData> InventoryItems;
	
	
	/**
	 * @brief Set to true when slots have been created. 
	 */
	UPROPERTY(BlueprintReadOnly, Category="Inventory Data")
	bool bHaveSlotsBeenInitialized;

	/**
	 * @brief Called when Inventory is updated
	 */
	UFUNCTION()
	void OnRep_InventoryItemsUpdated() const;

	/**
	 * @brief Called when InventorySlots is updated
	 */
	UFUNCTION()
	void OnRep_InventorySlotsUpdated() const;
	
	/**
	 * @brief Utilizes Inventory Size to reset array of InventorySlots.  All slots will be set to unoccupied.  
	 */
	void InitializeSlots();
	
	/**
	 * @brief Cycles through all slots until item is added.
	 * @param Item Item to add
	 * @return  Will return false if Item is not valid, weight cannot be added, or Item cannot fit in any positions
	 */
	bool AutoAddItemNewStack(FItemData Item);
	
	/**
	 * @brief Attempts to stack given Item Data into existing stack
	 * @param TargetItemData Item data to attempt to stack into
	 * @param InItemData Item data to stack into target item data
	 * @param OutRemainingItem Item data that could not be added to the stack
	 * @return Will return true if fully stacked, will return false if partially or not stack and return remaining item.
	 */
	bool AttemptStack(FInventoryItemData TargetItemData, FItemData InItemData, FItemData& OutRemainingItem);
	
	/**
	 * @brief Given a position, will update that slot's status to the NewIsOccupied.  Should mainly be used as a helper
	 * function for SetSlotStatuses which can update multiple slots at once and will use FInventorySlot delegate to broadcast
	 * @param TargetPosition Target position to update
	 * @param NewIsOccupied The new slot status for the position 
	 * @param bShouldBroadCast Should Broadcast will determine if FInventorySlot Update will be called.  Should be false
	 * unless being called directly to update one slot. 
	 * @return Will return false if the position could not be found in the InventorySlot array.
	 */
	bool SetSlotStatus(FInventory2D TargetPosition, bool NewIsOccupied, bool bShouldBroadCast = false);
	
	/**
	 * @brief Given an array of positions, will update the slot statuses to the NewIsOccupied and broadcast update
	 * @param TargetPositions Target slots to set status
	 * @param NewIsOccupied Status to set target slots
	 * @return Will return false if a position could not be found in the InventorySlot array.
	 */
	bool SetSlotStatuses(TArray<FInventory2D> TargetPositions, bool NewIsOccupied);
	
	/**
	 * @brief Given a target position, will return the InventorySlot reference from the InventorySlot Array.
	 * @param TargetPosition Target Position in inventory
	 * @param OutSlot Will return slot reference of given position
	 * @return Will return false if target position is not found in the array.
	 */
	bool FindSlotAtPosition(FInventory2D TargetPosition, FInventorySlot& OutSlot);
	
	/**
	 * @brief Given a target position, will return the index of that position from the InventorySlot Array.
	 * @param TargetPosition Target position to get array index
	 * @param OutIndex Array index
	 * @return Will return false if target position is not found in the array
	 */
	bool FindSlotAtPosition(FInventory2D TargetPosition, int32& OutIndex) const;
	
	/**
	 * @brief Helper function to ensures GUID is valid and the In World Class is not a nullptr.
	 * @param ItemData Item data to check
	 * @return Returns true if valid, returns false if not.
	 */
	static bool CheckIfItemValid(FItemData ItemData);
	
	/**
	 * @brief Checks to see if item can be added without exceeding the Inventory's max weight
	 * @param ItemData Item data to check
	 * @return Returns true if OK, false if not
	 */
	bool CheckIfItemWeightOK(FItemData ItemData) const;
	
	/**
	 * @brief Helper function that checks if the item is valid and that it's weight can be added
	 * @param ItemToCheck Item to check
	 * @return true if OK, false if not
	 */
	bool AddItemChecks(FItemData ItemToCheck) const;
	
	/**
	 * @brief Helper functions that performs checks before transferring items
	 * @param ItemToCheck Item to check if can be transferred
	 * @param InventoryToCheck target inventory to ensure not null
	 * @param TargetPosition target position in inventory to ensure item will fit
	 * @return returns true if OK, false if not
	 */
	bool TransferItemChecks(FInventoryItemData ItemToCheck, UInventoryComponent* InventoryToCheck, const FInventory2D TargetPosition) const;
	
	/**
	 * @brief Helper functions that performs checks before transferring items
	 * @param ItemToCheck Item to check if can be transferred
	 * @param InventoryToCheck target inventory to ensure not null
	 * @return returns true if OK, false if not
	 */
	bool TransferItemChecks(FInventoryItemData ItemToCheck, const UInventoryComponent* InventoryToCheck) const;
	
	/**
	 * @brief Helper function that performs checks before splitting an item stack
	 * @param Item Item to split
	 * @param QuantityTest Quantity to split from item
	 * @return returns true if OK, false if not
	 */
	bool SplitItemChecks(FInventoryItemData Item, int32 QuantityTest) const;
	
	/**
	 * @brief Adds the weight of Item's stack to the current weight.  Clamped between 0 and MaxWeight.
	 * @param ItemData Item data to get weight from
	 */
	void AddWeight(FItemData ItemData);
	
	/**
	 * @brief Adds a specific amount of weight to the current weight.  Usefully for when items are added directly into a
	 * stack
	 * @param AddWeight weight to add
	 */
	void AddWeight(float AddWeight);
	
	/**
	 * @brief Removes the weight of Item's stack from the current weight.  Clamped between 0 and MaxWeight.
	 * @param ItemData Item data to remove weight from current weight
	 */
	void RemoveWeight(FItemData ItemData);
	
	/**
	 * @brief Removes the weight from the current weight.  Clamped between 0 and MaxWeight.
	 * @param RemoveWeight weight to remove from current weight
	 */
	void RemoveWeight(float RemoveWeight);


	/** ------ DEBUGGING ------**/
	
	/**
	 * @brief boolean for if debugging items should be added at begin play
	 */
	UPROPERTY(EditDefaultsOnly,Category="Debugging")
	bool bAddDebugItems;

	
	/**
	 * @brief Array of debugging item data to be added at begin play
	 */
	UPROPERTY(EditDefaultsOnly,Category="Debugging", meta = (EditCondition = "bAddDebugItems"))
	TArray<TSubclassOf<AItemBase>> DebugItems;

	/**
	 * @brief Function to add debugging items to inventory at begin play
	 */
	void AddDebugItems();
	
	/** ------ RPCs ------ **/
	
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_MoveItem(FInventoryItemData TargetItem, FInventory2D TargetPosition, bool bRotateITem);
	bool Server_MoveItem_Validate(FInventoryItemData TargetItem, FInventory2D TargetPosition, bool bRotateITem);
	void Server_MoveItem_Implementation(FInventoryItemData TargetItem, FInventory2D TargetPosition, bool bRotateITem);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_SplitItem(FInventoryItemData TargetItemData, int32 NewStackQuantity);
	bool Server_SplitItem_Validate(FInventoryItemData TargetItemData, int32 NewStackQuantity);
	void Server_SplitItem_Implementation(FInventoryItemData TargetItemData, int32 NewStackQuantity);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_TransferItem(UInventoryComponent* TargetInventory,FInventoryItemData TargetItem);
	bool Server_TransferItem_Validate(UInventoryComponent* TargetInventory,FInventoryItemData TargetItem);
	void Server_TransferItem_Implementation(UInventoryComponent* TargetInventory,FInventoryItemData TargetItem);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_TransferItemToPosition(UInventoryComponent* TargetInventory, FInventory2D TargetPosition,
								FInventoryItemData TargetItem, bool bRotateItem);
	bool Server_TransferItemToPosition_Validate(UInventoryComponent* TargetInventory, FInventory2D TargetPosition,
								FInventoryItemData TargetItem, bool bRotateItem);
	void Server_TransferItemToPosition_Implementation(UInventoryComponent* TargetInventory, FInventory2D TargetPosition,
								FInventoryItemData TargetItem, bool bRotateItem);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_CombineStackSameInventory(FInventoryItemData OriginatingStack,FInventoryItemData TargetStack);
	bool Server_CombineStackSameInventory_Validate(FInventoryItemData OriginatingStack,FInventoryItemData TargetStack);
	void Server_CombineStackSameInventory_Implementation(FInventoryItemData OriginatingStack,FInventoryItemData TargetStack);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "PlayerInventory.generated.h"

class UPlayerInteractionSensor;

/**
 *@brief Base class for player inventory.  Used to extend inventory class for more player specific functions
 */

UCLASS(ClassGroup=(Inventory), Blueprintable, meta=(BlueprintSpawnableComponent))
class ITEMSYSTEM_API UPlayerInventory : public UInventoryComponent
{
	GENERATED_BODY()

public:
		
	/**
	 * @brief Starts process to place item in the world
	 * @param ItemData Inventory Item to place in the world
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void RequestPlaceItem(FInventoryItemData ItemData);

protected:

	/**
	 * @brief Spawn loop rate
	 */
	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	float SpawnLoopRate = 0.01f;

	/**
	 * @brief Increment to increase or decrease spawn yaw
	 */
	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	float PlacementRotationIncrements = 5.f;

	/**
	 * @brief Yaw for spawning actor
	 */
	FRotator SpawnYaw;
	
	/**
	 * @brief Pointer to item that is currently being placed.  May be null. 
	 */
	UPROPERTY()
	AItemBase* SpawningItem;

	/**
	 * @brief Reference to inventory item data that instigated placement process
	 */
	UPROPERTY()
	FInventoryItemData SpawningItemData;

	/**
	 * @brief Reference to player interaction sensor, will be used to get look and hit location 
	 */
	UPROPERTY()
	UPlayerInteractionSensor* InteractionSensor;
	
	/**
	 * @brief Handle for spawn loop timer
	 */
	FTimerHandle SpawnLoopTimer;

	/**
	 * @brief Begin Play
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Sets references, spawns ghost actor,  and begins spawn loop timer
	 */
	void StartItemSpawnLoop(FInventoryItemData ItemData);

	/**
	 * @brief Spawns ghost item for
	 * @param ItemData Used to spawn item
	 * @param OutSpawnedItem Out pointer for spawned item
	 */
	void SpawnGhostItem(FItemData ItemData, AItemBase*& OutSpawnedItem) const;

	/**
	 * @brief Spawn loop for moving item around in world before confirming placement
	 */
	void ItemSpawnLoop();
	
	/**
	 * @brief Placing Item should be called from inventory so this closes the UI
	 */
	void ClosePlayerUI() const;

	/**
	 * @brief Clears timer and unbinds delegates
	 */
	void EndSpawnLoop();
	
	/**
	 * @brief Destroys ghost actor
	 */
	UFUNCTION()
	void CancelPlacement();

	/**
	 * @brief Destroys actor and finalizes placement.  Will perform PRC if client
	 */
	UFUNCTION()
	void ConfirmPlacement();

	/**
	 * @brief Places final item in world.  Should only be called on server.  
	 * @param ItemData Item Data from inventory
	 * @param Transform Spawn Transform from ghost mesh
	 */
	void PlaceItem(FInventoryItemData ItemData,FTransform Transform);

	/**
	 * @brief Increases spawn yaw, bound to scroll wheel up
	 */
	UFUNCTION()
	void IncreaseSpawnYaw();

	/**
	 * @brief Decreases spawn yaw bound to scroll wheel down.
	 */
	UFUNCTION()
	void DecreaseSpawnYaw();

	/**
	* @brief RPC for clients to place item in the role
	*/ 
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_PlaceItem(FInventoryItemData ItemData,FTransform Transform);
	bool Server_PlaceItem_Validate(FInventoryItemData ItemData,FTransform Transform);
	void Server_PlaceItem_Implementation(FInventoryItemData ItemData,FTransform Transform);	
	
};

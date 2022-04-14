// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "GameFramework/PlayerController.h"
#include "StorageInventory.generated.h"

class UPlayerInventory;


/**
 * @brief Base class for Storage Inventories.  Extended from Inventory base class for more specific storage functionality
 */
UCLASS(ClassGroup=(Inventory), Blueprintable, meta=(BlueprintSpawnableComponent))
class ITEMSYSTEM_API UStorageInventory : public UInventoryComponent
{
	GENERATED_BODY()

public:

	//
	//
	/**
	 * @brief Validates player controller and set's owning actor ownership to the player controller.  This will facilitate RPC
	 * calls for the player.  Should only occur on the server and then do an RPC to the client to open the Transfer UI. 
	 * @param InstigatingPlayer Player that is opening the storage inventory
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly,Category="Inventory")
	void OpenInventory(APlayerController* InstigatingPlayer);

	/**
	 * @brief Called when the storage inventory is closed.  Will reset the owner of the storage actor
	 * @param InstigatingPlayer 
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void CloseInventory(APlayerController* InstigatingPlayer);
	
	/**
	 * @brief Used to interface with blueprint to create the storage inventory UI
	 * @param PlayerInventory Inventory of player opening the storage
	 * @param OwningPlayer Owning player controller reference 
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void AddTransferUI(UPlayerInventory* PlayerInventory, APlayerController* OwningPlayer);
	


protected:

	/**
	 * @brief boolean for if inventory is open
	 */
	UPROPERTY(Replicated,BlueprintReadOnly,Category="Inventory")
	bool bIsInventoryOpen;

	/**
	 * @brief RPC for closing the inventory when called by the client.
	 */
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_CloseInventory(APlayerController* InstigatingPlayer);
	bool Server_CloseInventory_Validate(APlayerController* InstigatingPlayer);
	void Server_CloseInventory_Implementation(APlayerController* InstigatingPlayer);
	
	/**
	 * @brief RPC for the server to tell the client to open inventory since opening storage request should only occur on
	 * the server
	 */
	UFUNCTION(Client,Reliable)
	void Client_AddTransferUI(UPlayerInventory* PlayerInventory,APlayerController* OwningPlayer);
	void Client_AddTransferUI_Implementation(UPlayerInventory* PlayerInventory,APlayerController* OwningPlayer);
	
};

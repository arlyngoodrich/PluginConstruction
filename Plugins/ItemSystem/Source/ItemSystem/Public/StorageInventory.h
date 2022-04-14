// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "GameFramework/PlayerController.h"
#include "StorageInventory.generated.h"

class UPlayerInventory;


/**
 * 
 */
UCLASS(ClassGroup=(Inventory), Blueprintable, meta=(BlueprintSpawnableComponent))
class ITEMSYSTEM_API UStorageInventory : public UInventoryComponent
{
	GENERATED_BODY()

public:

	//Validates player controller and set's owning actor ownership to the player controller.  This will facilitate RPC
	//calls for the player.  
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly,Category="Inventory")
	void OpenInventory(APlayerController* InstigatingPlayer);

	UFUNCTION(BlueprintCallable,Category="Inventory")
	void CloseInventory(APlayerController* InstigatingPlayer);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void AddTransferUI(UPlayerInventory* PlayerInventory, APlayerController* OwningPlayer);
	


protected:

	UPROPERTY(Replicated,BlueprintReadOnly,Category="Inventory")
	bool bIsInventoryOpen;

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_CloseInventory(APlayerController* InstigatingPlayer);
	bool Server_CloseInventory_Validate(APlayerController* InstigatingPlayer);
	void Server_CloseInventory_Implementation(APlayerController* InstigatingPlayer);
	
	UFUNCTION(Client,Reliable)
	void Client_AddTransferUI(UPlayerInventory* PlayerInventory,APlayerController* OwningPlayer);
	void Client_AddTransferUI_Implementation(UPlayerInventory* PlayerInventory,APlayerController* OwningPlayer);
	
};

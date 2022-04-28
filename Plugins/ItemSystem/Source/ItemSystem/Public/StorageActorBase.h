// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StorageActorBase.generated.h"

class UStorageInventory;
class UPlayerInventory;
class UStorageWidget;


/**
 * @brief Base class for any actor that can hold inventory items
 */
UCLASS()
class ITEMSYSTEM_API AStorageActorBase : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets default values for this actor's properties
	 */
	AStorageActorBase();

protected:
	/**
	 * @brief Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * @brief Called every frame
	 * @param DeltaTime time between frames
	 */
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStorageInventory* StorageInventory;

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
	 * @brief Used to interface with blueprint to create the storage inventory UI. Called from server to owning client so
	 * method is executed remotely.  
	 * @param PlayerInventory Inventory of player opening the storage
	 * @param OwningPlayer Owning player controller reference 
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Inventory")
	void AddTransferUI(UPlayerInventory* PlayerInventory, APlayerController* OwningPlayer);

	UFUNCTION(BlueprintNativeEvent,Category="Inventory")
	void RemoveTransferUI(APlayerController* InstigatingPlayer);
	


protected:

	/**
	 * @brief boolean for if inventory is open
	 */
	UPROPERTY(Replicated,BlueprintReadOnly,Category="Inventory")
	bool bIsInventoryOpen;

	UPROPERTY(EditDefaultsOnly, Category="Invetory")
	TSubclassOf<UStorageWidget> StorageWidgetClass;

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UStorageWidget* StorageWidget;

	bool CreateStorageWidget(APlayerController* OwningPlayer);
	
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

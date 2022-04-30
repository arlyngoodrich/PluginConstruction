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

protected:

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Inventory")
	void OnPlayerInteraction(APlayerController* InstigatingPlayer);

	UPROPERTY(BlueprintReadOnly,Replicated,Category="Inventory")
	bool bIsInventoryOpen;

	UPROPERTY(BlueprintReadOnly,Replicated,Category="Inventory")
	APlayerController* InteractingPlayer;

	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	TSubclassOf<UStorageWidget> StorageWidgetClass;

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UStorageWidget* StorageWidget;

	virtual bool CreateStorageWidget(APlayerController* InstigatingPlayer,UStorageWidget*& OutWidget);
	
	UFUNCTION()
	void OnInventoryForcedClosed();
	
	UFUNCTION(Client,Reliable)
	void Client_OpenInventory(APlayerController* InstigatingPlayer);
	void Client_OpenInventory_Implementation(APlayerController* InstigatingPlayer);

	UFUNCTION(Client,Reliable)
	void Client_CloseInventory(APlayerController* InstigatingPlayer);
	void Client_CloseInventory_Implementation(APlayerController* InstigatingPlayer);

	UFUNCTION(Server,Reliable)
	void Server_InventorForcedClosed();
	
	
};


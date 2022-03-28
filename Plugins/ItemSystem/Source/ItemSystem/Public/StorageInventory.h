// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "StorageInventory.generated.h"

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
	UFUNCTION(BlueprintAuthorityOnly,Category="Inventory")
	void OnInventoryInteract(APlayerController* InstigatingPlayer) const;
	
};

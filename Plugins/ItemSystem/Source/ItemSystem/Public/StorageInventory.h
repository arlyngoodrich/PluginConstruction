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
	
	
};

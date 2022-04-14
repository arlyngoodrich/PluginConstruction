// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "PlayerInventory.generated.h"

/**
 *@brief Base class for player inventory.  Used to extend inventory class for more player specific functions
 */

UCLASS(ClassGroup=(Inventory), Blueprintable, meta=(BlueprintSpawnableComponent))
class ITEMSYSTEM_API UPlayerInventory : public UInventoryComponent
{
	GENERATED_BODY()
	
};

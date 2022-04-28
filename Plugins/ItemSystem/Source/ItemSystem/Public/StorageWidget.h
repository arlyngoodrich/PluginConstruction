// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "StorageWidget.generated.h"

class UStorageInventory;
class UPlayerInventory;

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UStorageWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	void SetReferences(UStorageInventory* SetOwningStorageInventory,UPlayerInventory* SetPlayerInventory);

protected:

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UStorageInventory* OwningStorageInventory;

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	UPlayerInventory* PlayerInventory;
	
	
	
};

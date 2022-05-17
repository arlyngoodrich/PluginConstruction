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
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Inventory")
	void SetReferences(UStorageInventory* SetStorageInventory,UPlayerInventory* SetPlayerInventory,APlayerController* OwningPlayer);

protected:

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UStorageInventory* StorageInventory;

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UPlayerInventory* PlayerInventory;


	
};

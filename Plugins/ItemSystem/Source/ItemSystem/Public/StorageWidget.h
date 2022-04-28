// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "StorageWidget.generated.h"

class UStorageInventory;
class UPlayerInventory;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemoveFromParent);

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UStorageWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FOnRemoveFromParent OnRemoveFromParent;
	
	UFUNCTION(BlueprintNativeEvent,Category="Inventory")
	void SetReferences(UStorageInventory* SetStorageInventory,UPlayerInventory* SetPlayerInventory,APlayerController* OwningPlayer);

protected:

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UStorageInventory* StorageInventory;

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UPlayerInventory* PlayerInventory;

	virtual void RemoveFromParent() override;
	
};

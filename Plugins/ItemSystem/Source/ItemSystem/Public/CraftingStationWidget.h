// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "CraftingStationWidget.generated.h"

class UPlayerInventory;
class UStorageInventory;
class UStationCraftingComponent;

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UCraftingStationWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Inventory")
	void SetReferences(UPlayerInventory* SetPlayerInventory, UStorageInventory* SetStorageInventory,
		UStationCraftingComponent* SetCraftingComponent, APlayerController* OwningPlayer);

protected:

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UPlayerInventory* PlayerInventory;

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UStorageInventory* StorageInventory;

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UStationCraftingComponent* CraftingComponent;

	
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "InventoryData.h"
#include "InventoryItemWidget.generated.h"


class UInventoryGridWidget;
class UInventorySlotWidget;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UInventoryItemWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	//Inventory Item Data struct from the owning item.  Should be set before construct. 
	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	FInventoryItemData MyInventoryItemData;

	//Owning Grid Widget.  Should be set before construct. 
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	UInventoryGridWidget* OwningGridWidget;

	//Owning Widget Slot used to set position.  Should be set before construct.  
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	UInventorySlotWidget* OwningSlot;

	//Owning Owning Inventory Component.  Should be set before cosntruct.  
	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	UInventoryComponent* OwningInventory;
	
};

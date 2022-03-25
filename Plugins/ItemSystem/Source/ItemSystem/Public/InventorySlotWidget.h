// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "InventoryData.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UInventorySlotWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	FInventorySlot MyInventorySlot;

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	class UInventoryGridWidget* OwningGridWidget;

	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	class UInventoryComponent* OwningInventory;

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	bool bDraggedOver;
	
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void OnSlotDraggedOver(FItemData DraggedOverItem,bool bRotateItem);

	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool OnItemDropped(FInventoryItemData DroppedItemData, bool bRotateItem);

};

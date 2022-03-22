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
	


	
};

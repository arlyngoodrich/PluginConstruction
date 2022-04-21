// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "ItemManagementWidget.generated.h"

class UInventoryComponent;
class UCraftingComponent;

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UItemManagementWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite,Category="Item Management",meta = (ExposeOnSpawn))
	TArray<UInventoryComponent*> PlayerInventoryComponents;

	UPROPERTY(BlueprintReadWrite,Category="Item Management",meta = (ExposeOnSpawn))
	UCraftingComponent* PlayerCraftingComponent;
	
};

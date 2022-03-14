// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryData.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent) )
class ITEMSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Inventory Data")
	FText InventoryName;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Invetory Data")
	FInventory2D InventorySize;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Inventory Data")
	float MaxWeight;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory Data")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(Replicated, BlueprintReadOnly,Category="Inventory Data");
	TArray<FInventoryItemData> InventoryItems;

	void InitializeSlots();

	void SetInventory(TArray<FInventoryItemData> InInventoryItems);

	bool CheckIfItemFits(FInventoryItemData InventoryItemData, FInventory2D TargetPosition);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
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
	FInventory2D InventorySize;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Inventory Data")
	float MaxWeight;

	UPROPERTY(BlueprintReadOnly,Category="Inventory Data")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(BlueprintReadOnly,Category="Inventory Data")
	TArray<FInventoryItemData> Inventory;

	void InitializeSlots();
	
};

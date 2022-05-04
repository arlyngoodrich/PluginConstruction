// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "PlayerInventory.generated.h"

class UPlayerInteractionSensor;

/**
 *@brief Base class for player inventory.  Used to extend inventory class for more player specific functions
 */

UCLASS(ClassGroup=(Inventory), Blueprintable, meta=(BlueprintSpawnableComponent))
class ITEMSYSTEM_API UPlayerInventory : public UInventoryComponent
{
	GENERATED_BODY()

public:
		
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void PlaceItem(FItemData ItemData);

protected:

	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	float SpawnLoopRate = 0.01f;
	
	UPROPERTY()
	AItemBase* SpawningItem;

	UPROPERTY()
	UPlayerInteractionSensor* InteractionSensor;
	
	FTimerHandle SpawnLoopTimer;

	virtual void BeginPlay() override;
	
	void StartItemSpawnLoop(FItemData SpawnItemData);

	void SpawnItem(FItemData ItemData, AItemBase*& OutSpawnedItem) const;

	void ItemSpawnLoop();
	
	void ClosePlayerUI() const;

	void EndSpawnLoop();
	
	UFUNCTION()
	void CancelPlacement();

	UFUNCTION()
	void ConfirmPlacement();
	

	
	
};

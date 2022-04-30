// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingStationWidget.h"

void UCraftingStationWidget::SetReferences_Implementation(UPlayerInventory* SetPlayerInventory, UStorageInventory* SetStorageInventory,
	UStationCraftingComponent* SetCraftingComponent, APlayerController* OwningPlayer)
{
	PlayerInventory = SetPlayerInventory;
	StorageInventory = SetStorageInventory;
	CraftingComponent = SetCraftingComponent;
	SetOwningPlayer(OwningPlayer);
}

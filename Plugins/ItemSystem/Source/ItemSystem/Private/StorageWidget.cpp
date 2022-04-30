// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageWidget.h"


void UStorageWidget::SetReferences_Implementation(UStorageInventory* SetStorageInventory,
	UPlayerInventory* SetPlayerInventory, APlayerController* OwningPlayer)
{
	StorageInventory = SetStorageInventory;
	PlayerInventory = SetPlayerInventory;
	SetOwningPlayer(OwningPlayer);
}


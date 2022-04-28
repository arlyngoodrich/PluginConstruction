// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageWidget.h"

void UStorageWidget::SetReferences(UStorageInventory* SetOwningStorageInventory, UPlayerInventory* SetPlayerInventory)
{
	OwningStorageInventory = SetOwningStorageInventory;
	PlayerInventory = SetPlayerInventory;
}

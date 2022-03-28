// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageInventory.h"

void UStorageInventory::OnInventoryInteract(APlayerController* InstigatingPlayer) const
{
	if(GetOwnerRole()!=ROLE_Authority)
	{
		return;
	}

	if(InstigatingPlayer != nullptr)
	{
		GetOwner()->SetOwner(Cast<AActor>(InstigatingPlayer));
	}
	
}

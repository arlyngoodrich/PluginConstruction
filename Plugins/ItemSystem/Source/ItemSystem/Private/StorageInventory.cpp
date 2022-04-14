// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageInventory.h"

#include "ItemSystem.h"
#include "PlayerInventory.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"


void UStorageInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStorageInventory, bIsInventoryOpen);
}



void UStorageInventory::OpenInventory(APlayerController* InstigatingPlayer)
{
	if(GetOwnerRole()!=ROLE_Authority)
	{
		return;
	}

	if(InstigatingPlayer != nullptr)
	{
		GetOwner()->SetOwner(Cast<AActor>(InstigatingPlayer));
		bIsInventoryOpen = true;
		
		UE_LOG(LogItemSystem,Log,TEXT("%s set as owner of %s"),
			*InstigatingPlayer->GetName(),*GetOwner()->GetName())

		//Check to see if player has inventory
		const APawn* PlayerOwnedPawn = InstigatingPlayer->GetPawn();

		if (UPlayerInventory* PlayerInventory = PlayerOwnedPawn->FindComponentByClass<UPlayerInventory>();
			PlayerInventory != nullptr)
		{
			Client_AddTransferUI(PlayerInventory,InstigatingPlayer);
		}
	}
	
}

void UStorageInventory::CloseInventory(APlayerController* InstigatingPlayer)
{

	if(GetOwnerRole()!=ROLE_Authority)
	{
		Server_CloseInventory(InstigatingPlayer);
		return;
	}

	//Ensure same controller is closing inventory
	if(GetOwner()->GetOwner() == InstigatingPlayer)
	{
		GetOwner()->SetOwner(nullptr);
		bIsInventoryOpen = false;

		UE_LOG(LogItemSystem,Log,TEXT("%s is not longer the owner of %s"),
			*InstigatingPlayer->GetName(),*GetOwner()->GetName())
		
	}
}

bool UStorageInventory::Server_CloseInventory_Validate(APlayerController* InstigatingPlayer)
{
	return InstigatingPlayer == GetOwner()->GetOwner();
}

void UStorageInventory::Server_CloseInventory_Implementation(APlayerController* InstigatingPlayer)
{
	CloseInventory(InstigatingPlayer);
}

void UStorageInventory::Client_AddTransferUI_Implementation(UPlayerInventory* PlayerInventory,APlayerController* OwningPlayer)
{
	AddTransferUI(PlayerInventory,OwningPlayer);
}

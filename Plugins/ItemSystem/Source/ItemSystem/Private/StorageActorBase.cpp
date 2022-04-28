// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageActorBase.h"
#include "StorageInventory.h"
#include "Net/UnrealNetwork.h"
#include "ItemSystem.h"
#include "PlayerInventory.h"


// Sets default values
AStorageActorBase::AStorageActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StorageInventory = CreateDefaultSubobject<UStorageInventory>(TEXT("Storage Inventory"));
	StorageInventory->SetIsReplicated(true);

	SetReplicates(true);
	
}

// Called when the game starts or when spawned
void AStorageActorBase::BeginPlay()
{
	Super::BeginPlay();
}



// Called every frame
void AStorageActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStorageActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStorageActorBase, bIsInventoryOpen);
}



void AStorageActorBase::OpenInventory(APlayerController* InstigatingPlayer)
{
	if(HasAuthority())
	{
		return;
	}

	if(bIsInventoryOpen)
	{
		UE_LOG(LogItemSystem,Log,TEXT("Cannot open %s becuase a player already has inventory open"),
			*GetOwner()->GetName())
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

void AStorageActorBase::CloseInventory(APlayerController* InstigatingPlayer)
{

	if(HasAuthority())
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

bool AStorageActorBase::Server_CloseInventory_Validate(APlayerController* InstigatingPlayer)
{
	return InstigatingPlayer == GetOwner()->GetOwner();
}

void AStorageActorBase::Server_CloseInventory_Implementation(APlayerController* InstigatingPlayer)
{
	CloseInventory(InstigatingPlayer);
}

void AStorageActorBase::Client_AddTransferUI_Implementation(UPlayerInventory* PlayerInventory,APlayerController* OwningPlayer)
{
	AddTransferUI(PlayerInventory,OwningPlayer);
}

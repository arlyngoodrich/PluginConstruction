// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageActorBase.h"

#include "CustomUserWidget.h"
#include "StorageInventory.h"
#include "Net/UnrealNetwork.h"
#include "ItemSystem.h"
#include "PlayerInventory.h"
#include "CustomWidgetTemplates/Public/UIPlayerInterface.h"
#include "StorageWidget.h"


// Sets default values
AStorageActorBase::AStorageActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Inventory Reference
	StorageInventory = CreateDefaultSubobject<UStorageInventory>(TEXT("Storage Inventory"));
	StorageInventory->SetIsReplicated(true);

	//Ensure Actor Replicates
	SetReplicates(true);

	StorageWidgetClass = UStorageWidget::StaticClass();

	
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
	DOREPLIFETIME(AStorageActorBase, InteractingPlayer);
}

void AStorageActorBase::OnPlayerInteraction(APlayerController* InstigatingPlayer)
{
	//Check to see if inventory is already open
	if(bIsInventoryOpen)
	{
		//If yes, then check to see if the same as the current interacting player
		if(InstigatingPlayer==InteractingPlayer)
		{
			//If the same, then close the inventory and set net owner to null
			Client_CloseInventory(InstigatingPlayer);
			SetOwner(nullptr);
			bIsInventoryOpen = false;
			InteractingPlayer = nullptr;
			
		}
		else
		{
			//If different, then don't open inventory
			UE_LOG(LogItemSystem,Log,TEXT("Cannot open inventory %s inventory for %s.  %s alrady has it open"),
				*GetName(),*InstigatingPlayer->GetName(),*InteractingPlayer->GetName())
		}
	}
	else
	{
		//Open inventory for player and set net owner
		InteractingPlayer = InstigatingPlayer;
		SetOwner(InstigatingPlayer);
		bIsInventoryOpen = true;
		Client_OpenInventory(InteractingPlayer);
	}
}

void AStorageActorBase::OnInventoryForcedClosed()
{
	StorageWidget->OnRemoveFromParent.RemoveDynamic(this,&AStorageActorBase::OnInventoryForcedClosed);
	StorageWidget = nullptr;
	Server_InventorForcedClosed();
}

void AStorageActorBase::Client_OpenInventory_Implementation(APlayerController* InstigatingPlayer)
{
	APawn* Pawn = InstigatingPlayer->GetPawn();
	//Make sure player has inventory
	if(UPlayerInventory* PlayerInventory = Pawn->FindComponentByClass<UPlayerInventory>())
	{
		//Create Storage Widget
		StorageWidget = Cast<UStorageWidget>(CreateWidget(InstigatingPlayer,StorageWidgetClass));
		if(StorageWidget)
		{
			StorageWidget->SetReferences(StorageInventory,PlayerInventory,InstigatingPlayer);

			//Tell player to open UI via interface
			if(Pawn->GetClass()->ImplementsInterface(UUIPlayerInterface::StaticClass()))
			{
				IUIPlayerInterface::Execute_OpenUI(Pawn,StorageWidget);
				StorageWidget->OnRemoveFromParent.AddDynamic(this,&AStorageActorBase::OnInventoryForcedClosed);
			}
		}
		else
		{
			UE_LOG(LogItemSystem,Warning,TEXT("%s failed to make storage widget"),*GetName())
		}
	}
	else
	{
		UE_LOG(LogItemSystem,Warning,TEXT("%s player does not have inventory to create transfer widget"))
	}
}

void AStorageActorBase::Client_CloseInventory_Implementation(APlayerController* InstigatingPlayer)
{
	if(InstigatingPlayer == nullptr)
	{
		UE_LOG(LogItemSystem,Warning,TEXT("%s attempted to close inventory when InstigatingPlayer is null"),*GetName())
		return;
	}

	if(StorageWidget == nullptr)
	{
		UE_LOG(LogItemSystem,Warning,TEXT("%s attempted to close inventory when storage widget is null"),*GetName())
		return;
	}

	APawn* Pawn = InstigatingPlayer->GetPawn();
	//Tell player to open UI via interface
	if(Pawn->GetClass()->ImplementsInterface(UUIPlayerInterface::StaticClass()))
	{
		IUIPlayerInterface::Execute_CloseUI(Pawn,StorageWidget);
		//On Inventory Forced close will call when widget is removed from parent
	}
	else
	{
		UE_LOG(LogItemSystem,Warning,TEXT("%s could not close inventory. Interacting player does not have UI Interface"),
			*GetName())
	}
}


void AStorageActorBase::Server_InventorForcedClosed_Implementation()
{
	SetOwner(nullptr);
	bIsInventoryOpen = false;
	InteractingPlayer = nullptr;
}




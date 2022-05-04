// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"

#include "CustomPlayerController.h"
#include "ItemBase.h"
#include "ItemSystem.h"
#include "PlayerInteractionSensor.h"
#include "UIPlayerInterface.h"
#include "GameFramework/Character.h"


void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInventory::PlaceItem(const FItemData ItemData)
{
	StartItemSpawnLoop(ItemData);
}

void UPlayerInventory::StartItemSpawnLoop(const FItemData SpawnItemData)
{

	//Make sure Spawn Item class is valid
	if(SpawnItemData.InWorldClass == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to sapwn %s item from %s inventory without a valid in world class"),
			*SpawnItemData.DisplayName.ToString(),*GetOwner()->GetName())
		return;
	}

	//Make sure have interaction sensor
	InteractionSensor = GetOwner()->FindComponentByClass<UPlayerInteractionSensor>();
	if(InteractionSensor == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to sapwn %s item from %s inventory but player does not have an Interaction Sensor"),
	*SpawnItemData.DisplayName.ToString(),*GetOwner()->GetName())
		return;
	}

	//Check if custom player controller
	if (ACustomPlayerController* CustomPlayerController = Cast<ACustomPlayerController>(
		Cast<ACharacter>(GetOwner())->GetController()))
	{
		CustomPlayerController->OnRMBPressed.AddDynamic(this,&UPlayerInventory::CancelPlacement);
		CustomPlayerController->OnLMBPressed.AddDynamic(this,&UPlayerInventory::ConfirmPlacement);
	}

	ClosePlayerUI();

	SpawnItem(SpawnItemData,SpawningItem);
	
	GetWorld()->GetTimerManager().SetTimer(SpawnLoopTimer,this,&UPlayerInventory::ItemSpawnLoop,SpawnLoopRate,true,.1f);
}

void UPlayerInventory::SpawnItem(const FItemData ItemData, AItemBase*& OutSpawnedItem) const
{
	const FVector Location = InteractionSensor->GetLookLocation();
	const FRotator Rotation(0.f,0.f,0.f);
	const FActorSpawnParameters SpawnParameters;
	
	
	OutSpawnedItem  = GetWorld()->SpawnActor<AItemBase>(ItemData.InWorldClass,Location,Rotation,SpawnParameters);
	if(OutSpawnedItem == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("%s could not spawn %s item from their inventory"), *GetOwner()->GetName(),
	   *ItemData.DisplayName.ToString())
	}

	OutSpawnedItem->SetActorEnableCollision(false);

}

// ReSharper disable once CppMemberFunctionMayBeConst
void UPlayerInventory::ItemSpawnLoop()
{
	if(SpawningItem==nullptr)
	{
		return;
	}
	
	SpawningItem->SetActorLocation(InteractionSensor->GetLookLocation());
}

void UPlayerInventory::ClosePlayerUI() const
{
	
	if(GetOwner()->Implements<UUIPlayerInterface>())
	{
		IUIPlayerInterface::Execute_CloseActiveUI(GetOwner());
	}
	
}

void UPlayerInventory::EndSpawnLoop()
{
	//Check if custom player controller
	if (ACustomPlayerController* CustomPlayerController = Cast<ACustomPlayerController>(
		Cast<ACharacter>(GetOwner())->GetController()))
	{
		CustomPlayerController->OnRMBPressed.RemoveDynamic(this,&UPlayerInventory::CancelPlacement);
		CustomPlayerController->OnLMBPressed.RemoveDynamic(this,&UPlayerInventory::ConfirmPlacement);
	}
	
	GetWorld()->GetTimerManager().ClearTimer(SpawnLoopTimer);
}

void UPlayerInventory::CancelPlacement()
{
	EndSpawnLoop();
	SpawningItem->Destroy();
}

void UPlayerInventory::ConfirmPlacement()
{
	EndSpawnLoop();
}



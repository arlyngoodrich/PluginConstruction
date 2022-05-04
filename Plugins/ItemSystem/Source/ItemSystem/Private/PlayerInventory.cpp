// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"

#include "ItemBase.h"
#include "ItemSystem.h"
#include "PlayerInteractionSensor.h"




void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInventory::PlaceItem(FItemData ItemData)
{
}

void UPlayerInventory::StartItemSpawnLoop(const FItemData SpawnItemData)
{

	if(SpawnItemData.InWorldClass == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to sapwn %s item from %s inventory without a valid in world class"),
			*SpawnItemData.DisplayName.ToString(),*GetOwner()->GetName())
		return;
	}

	InteractionSensor = GetOwner()->FindComponentByClass<UPlayerInteractionSensor>();
	if(InteractionSensor == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to sapwn %s item from %s inventory but player does not have an Interaction Sensor"),
	*SpawnItemData.DisplayName.ToString(),*GetOwner()->GetName())
		return;
	}
	
	
	GetWorld()->GetTimerManager().SetTimer(SpawnLoopTimer,this,&UPlayerInventory::ItemSpawnLoop,SpawnLoopRate,true);
}

void UPlayerInventory::SpawnItem(const FItemData ItemData, AItemBase*& OutSpawnedItem) const
{
	const FVector Location = InteractionSensor->GetLookLocation();
	const FRotator Rotation(0.f,0.f,0.f);
	const FActorSpawnParameters SpawnParameters;

	UClass* ItemClass = ItemData.InWorldClass->GetClass();
	OutSpawnedItem  = GetWorld()->SpawnActor<AItemBase>(ItemClass,Location,Rotation,SpawnParameters);
}

void UPlayerInventory::ItemSpawnLoop()
{
	
}



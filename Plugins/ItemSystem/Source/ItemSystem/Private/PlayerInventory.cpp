// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"

#include "CustomPlayerController.h"
#include "ItemBase.h"
#include "ItemSystem.h"
#include "PlayerInteractionSensor.h"
#include "UIPlayerInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInventory::RequestPlaceItem(const FInventoryItemData ItemData)
{
	StartItemSpawnLoop(ItemData);
}

void UPlayerInventory::StartItemSpawnLoop(const FInventoryItemData ItemData)
{

	//Make sure Spawn Item class is valid
	if(ItemData.Item.InWorldClass == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to sapwn %s item from %s inventory without a valid in world class"),
			*ItemData.Item.DisplayName.ToString(),*GetOwner()->GetName())
		return;
	}

	//Make sure have interaction sensor
	InteractionSensor = GetOwner()->FindComponentByClass<UPlayerInteractionSensor>();
	if(InteractionSensor == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("Attempted to sapwn %s item from %s inventory but player does not have an Interaction Sensor"),
	*ItemData.Item.DisplayName.ToString(),*GetOwner()->GetName())
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

	SpawnItem(ItemData.Item,SpawningItem);
	SpawningItemData = ItemData;
	
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

	//Set all mesh components to ignore visibility so it doesn't show up in trace
	TArray<UMeshComponent*> MeshComponents;
	OutSpawnedItem->GetComponents<UMeshComponent>(MeshComponents);
	for (int i = 0; i < MeshComponents.Num(); ++i)
	{
		MeshComponents[i]->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);
	}

}

// ReSharper disable once CppMemberFunctionMayBeConst
void UPlayerInventory::ItemSpawnLoop()
{
	if(SpawningItem==nullptr)
	{
		return;
	}

	const FHitResult LookHit = InteractionSensor->GetLookHitResult();

	SpawningItem->SetActorLocation(InteractionSensor->GetLookLocation());

	if(LookHit.bBlockingHit)
	{
		SpawningItem->SetActorRotation(UKismetMathLibrary::MakeRotFromZ(LookHit.ImpactNormal));
	}

	
	
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

	if(GetOwnerRole() != ROLE_Authority)
	{
		Server_PlaceItem(SpawningItemData,SpawningItem->GetTransform());
	}
	else
	{
		PlaceItem(SpawningItemData,SpawningItem->GetTransform());
	}

	SpawningItem->Destroy();
	SpawningItem = nullptr;
	
}

void UPlayerInventory::PlaceItem(const FInventoryItemData ItemData, const FTransform Transform) const
{

	if(GetOwnerRole()!=ROLE_Authority){return;}
	
	const FActorSpawnParameters SpawnParameters;
	AItemBase* SpawnedItem = GetWorld()->SpawnActor<AItemBase>(ItemData.Item.InWorldClass, Transform, SpawnParameters);
	SpawnedItem->SetItemData(ItemData.Item);
}

bool UPlayerInventory::Server_PlaceItem_Validate(FInventoryItemData ItemData, FTransform Transform)
{
	return true;
}

void UPlayerInventory::Server_PlaceItem_Implementation(FInventoryItemData ItemData,FTransform Transform)
{
	PlaceItem(ItemData,Transform);
}



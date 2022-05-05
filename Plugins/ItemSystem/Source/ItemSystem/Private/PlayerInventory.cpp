// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"

#include "CustomPlayerController.h"
#include "ItemBase.h"
#include "ItemSystem.h"
#include "PlayerInteractionSensor.h"
#include "UIPlayerInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstance.h"
#include "UObject/ConstructorHelpers.h"


UPlayerInventory::UPlayerInventory()
{
	//Get good spawn default material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
	GoodMaterial(TEXT("MaterialInstanceConstant'/ItemSystem/Materials/MI_GhostItemPlacement_OK.MI_GhostItemPlacement_OK'"));

	if(GoodMaterial.Object)
	{
		SpawnOKMaterial = Cast<UMaterialInterface>(GoodMaterial.Object);
	}
	else
	{
		UE_LOG(LogItemSystem,Warning,TEXT("%s could not set default good spawn material"),*GetClass()->GetName())
	}

	//Get bad spawn default material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
	BadMaterial(TEXT("MaterialInstanceConstant'/ItemSystem/Materials/MI_GhostItemPlacement_Bad.MI_GhostItemPlacement_Bad'"));

	if(BadMaterial.Object)
	{
		SpawnBadMaterial = Cast<UMaterialInterface>(BadMaterial.Object);
	}
	else
	{
		UE_LOG(LogItemSystem,Warning,TEXT("%s could not set default bad spawn material"),*GetClass()->GetName())
	}


}

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
		if(CustomPlayerController->IsLocalController())
		{
			CustomPlayerController->OnRMBPressed.AddDynamic(this,&UPlayerInventory::CancelPlacement);
			CustomPlayerController->OnLMBPressed.AddDynamic(this,&UPlayerInventory::ConfirmPlacement);
			CustomPlayerController->OnMouseScrollUp.AddDynamic(this,&UPlayerInventory::IncreaseSpawnYaw);
			CustomPlayerController->OnMouseScrollDown.AddDynamic(this,&UPlayerInventory::DecreaseSpawnYaw);
		}
		else
		{
			return;
		}

	}
	
	ClosePlayerUI();

	SpawnGhostItem(ItemData.Item,SpawningItem);
	SpawningItemData = ItemData;
	SpawningItem->OnPlacementStart();
	InteractionSensor->ToggleInteraction(false);
	
	GetWorld()->GetTimerManager().SetTimer(SpawnLoopTimer,this,&UPlayerInventory::ItemSpawnLoop,SpawnLoopRate,true,.1f);
}

void UPlayerInventory::SpawnGhostItem(const FItemData ItemData, AItemBase*& OutSpawnedItem) const
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

		//Set Spawn OK material
		if(SpawnOKMaterial)
		{
			const int32 NumMaterials = MeshComponents[i]->GetNumMaterials();

			for (int n = 0; n < NumMaterials; ++n)
			{
				MeshComponents[i]->SetMaterial(n,SpawnOKMaterial);
			}
		}
	}

	OutSpawnedItem->SetOwner(GetOwner());
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
		FRotator NewRotation = UKismetMathLibrary::MakeRotFromZ(LookHit.ImpactNormal);
		NewRotation.Yaw = SpawnYaw.Yaw;
		SpawningItem->SetActorRotation(NewRotation);
		
	}

	bSpawnOk = CheckIfSpawnOK(SpawningItem);

	UMaterialInterface* SpawnMaterial;
	if(bSpawnOk)
	{
		SpawnMaterial = SpawnOKMaterial;
	}
	else
	{
		SpawnMaterial = SpawnBadMaterial;
	}

	if(SpawnMaterial)
	{
		TArray<UMeshComponent*> MeshComponents;
		SpawningItem->GetComponents<UMeshComponent>(MeshComponents);
		for (int i = 0; i < MeshComponents.Num(); ++i)
		{
			UMeshComponent* TargetMesh = MeshComponents[i];
			const int32 NumMaterials = TargetMesh->GetNumMaterials();

			for (int n = 0; n < NumMaterials; ++n)
			{
				TargetMesh->SetMaterial(n,SpawnMaterial);
			}
		}
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
		CustomPlayerController->OnMouseScrollUp.RemoveDynamic(this,&UPlayerInventory::IncreaseSpawnYaw);
		CustomPlayerController->OnMouseScrollDown.RemoveDynamic(this,&UPlayerInventory::DecreaseSpawnYaw);
	}

	InteractionSensor->ToggleInteraction(true);
	
	GetWorld()->GetTimerManager().ClearTimer(SpawnLoopTimer);
}

void UPlayerInventory::CancelPlacement()
{
	EndSpawnLoop();
	SpawningItem->Destroy();
}

void UPlayerInventory::ConfirmPlacement()
{
	if(bSpawnOk == true)
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
	
}

void UPlayerInventory::PlaceItem(const FInventoryItemData ItemData, const FTransform Transform)
{

	if(GetOwnerRole()!=ROLE_Authority){return;}


	if(FullyRemoveInventoryItem(ItemData))
	{
		const FActorSpawnParameters SpawnParameters;
		AItemBase* SpawnedItem = GetWorld()->SpawnActor<AItemBase>(ItemData.Item.InWorldClass, Transform, SpawnParameters);
		SpawnedItem->SetItemData(ItemData.Item);

		UE_LOG(LogItemSystem,Log,TEXT("%s placed %s item from inventory"),
		*GetOwner()->GetName(),*ItemData.Item.DisplayName.ToString());
	}
	else
	{
		UE_LOG(LogItemSystem,Warning,TEXT("%s attempted to spawn %s item but it could not be removed from inventory"),
			*GetOwner()->GetName(),*ItemData.Item.DisplayName.ToString());
	}

	
}

void UPlayerInventory::IncreaseSpawnYaw()
{
	const float CurrentYaw = SpawnYaw.Yaw;
	SpawnYaw.Yaw =CurrentYaw + PlacementRotationIncrements;
}

void UPlayerInventory::DecreaseSpawnYaw()
{
	const float CurrentYaw = SpawnYaw.Yaw;
	SpawnYaw.Yaw = CurrentYaw - PlacementRotationIncrements;
}

bool UPlayerInventory::CheckIfSpawnOK(AItemBase* ItemToCheck) const
{
	//Check if Pitch or roll is greater than max placement angle
	const FRotator Rotator = ItemToCheck->GetActorRotation();
	if(abs(Rotator.Pitch) > MaxPlacementAngle || abs(Rotator.Roll) > MaxPlacementAngle)
	{
		return false;
	}

	//Check if there are more than one overlapping actors
	ItemToCheck->UpdateOverlaps();
	TArray<AActor*> Actors;
	ItemToCheck->GetOverlappingActors(Actors);
	
	if(Actors.Num()>1)
	{
		return false;
	}

	return true;
	
}



bool UPlayerInventory::Server_PlaceItem_Validate(FInventoryItemData ItemData, FTransform Transform)
{
	return true;
}

void UPlayerInventory::Server_PlaceItem_Implementation(FInventoryItemData ItemData,FTransform Transform)
{
	PlaceItem(ItemData,Transform);
}



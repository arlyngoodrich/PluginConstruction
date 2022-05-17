// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "InteractableObjectComponent.h"
#include "InventoryComponent.h"
#include "ItemSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	InteractableObjectComponent = CreateDefaultSubobject<UInteractableObjectComponent>(TEXT("Interactable Object Component"));
	InteractableObjectComponent->bShouldOutline = true;
	InteractableObjectComponent->bShouldShowWidget = true;
	InteractableObjectComponent->InteractionText = FText::FromString("Pickup");
}

FItemData AItemBase::GetItemData() const {return ItemData;}

void AItemBase::SetItemData(const FItemData SetItemData)
{
	if(HasAuthority())
	{
		ItemData = SetItemData;
	}
	else
	{
		UE_LOG(LogItemSystem,Warning,TEXT("%s attempted to set item data from non authority"),*GetName())
	}
	
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeItemData();
	InteractableObjectComponent->OnInteractionTriggered.AddDynamic(this,&AItemBase::OnPlayerInteraction);
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::OnPlacementStart()
{
	if(HasAuthority())
	{
		SetReplicates(false);
		if(GetLocalRole()==ROLE_AutonomousProxy)
		{
			Destroy();
		}
	}

}

void AItemBase::StartPhysicsTimer()
{
	if(HasAuthority() == false){return;}
	
	StartPhysics();
	
	//Start timer to end physics
	GetWorld()->GetTimerManager().SetTimer(PhysicsTimerHandle,this,&AItemBase::EndPhysics,PhysicsTimeDuration,false);
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, ItemData);
}

void AItemBase::OnPlayerInteraction_Implementation(AActor* InstigatingActor)
{
	Native_OnPlayerInteraction(InstigatingActor);
}

void AItemBase::InitializeItemData()
{
	if(HasAuthority())
	{
		ItemData.ItemGuid = FGuid::NewGuid();
		ItemData.InWorldClass = this->GetClass();
		ItemData.bIsValid = true;

		UE_LOG(LogItemSystem, Log, TEXT("%s item intialized.  GUID = %s"), *ItemData.DisplayName.ToString(),
		       *ItemData.ItemGuid.ToString())
	}
}

void AItemBase::Native_OnPlayerInteraction(AActor* InstigatingActor)
{
	if(HasAuthority() == false || InstigatingActor == nullptr){return;}
	
	if(UInventoryComponent* InventoryComponent = InstigatingActor->FindComponentByClass<UInventoryComponent>())
	{
		FItemData RemainingItemData;
		if(InventoryComponent->AutoAddItem(ItemData,RemainingItemData))
		{
			//Fully Added
			SetLifeSpan(.01);
		}
		else
		{
			ItemData = RemainingItemData;
		}
	}
}

void AItemBase::StartPhysics()
{
	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);
	SetReplicatingMovement(true);

	for (int i = 0; i < MeshComponents.Num(); ++i)
	{
		MeshComponents[i]->SetSimulatePhysics(true);
	}
}

void AItemBase::EndPhysics()
{
	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);

	for (int i = 0; i < MeshComponents.Num(); ++i)
	{
		MeshComponents[i]->SetSimulatePhysics(false);
	}

	SetReplicatingMovement(false);
}




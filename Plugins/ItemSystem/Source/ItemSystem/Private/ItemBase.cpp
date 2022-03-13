// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "ItemSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Item Data defaults
	ItemData.DisplayName = FText::FromString("Base Item");
	ItemData.ItemHealth = 1.f;
	ItemData.ItemQuantity = 1;
	ItemData.PerItemWeight = 1.f;
	ItemData.bShouldItemStack = false;
	ItemData.ItemSize = FInventory2D(1,1);
	
	

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeItemData();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, ItemData);
}

void AItemBase::InitializeItemData()
{
	if(HasAuthority())
	{
		ItemData.ItemGuid = FGuid::NewGuid();
		ItemData.InWorldClass = this->GetClass();

		UE_LOG(LogItemSystem, Log, TEXT("%s item intialized.  GUID = %s"), *ItemData.DisplayName.ToString(),
		       *ItemData.ItemGuid.ToString())
	}
}

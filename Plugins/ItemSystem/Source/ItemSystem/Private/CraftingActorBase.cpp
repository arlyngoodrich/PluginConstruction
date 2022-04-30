// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingActorBase.h"

#include "CraftingStationWidget.h"
#include "ItemSystem.h"
#include "PlayerInventory.h"
#include "StorageWidget.h"
#include "Blueprint/UserWidget.h"

ACraftingActorBase::ACraftingActorBase()
{
	CraftingComponent = CreateDefaultSubobject<UStationCraftingComponent>(TEXT("Crafting Component"));
}

bool ACraftingActorBase::CreateStorageWidget(APlayerController* InstigatingPlayer, UCustomUserWidget*& OutWidget)
{
	
	const APawn* Pawn = InstigatingPlayer->GetPawn();
	
	if(UPlayerInventory* PlayerInventory = Pawn->FindComponentByClass<UPlayerInventory>())
	{
		UCraftingStationWidget* CraftingWidget = Cast<UCraftingStationWidget>(CreateWidget(InstigatingPlayer, StorageWidgetClass));
		if(CraftingWidget)
		{
			CraftingWidget->SetReferences(PlayerInventory,StorageInventory,CraftingComponent,InstigatingPlayer);
		}
		
		OutWidget = CraftingWidget;
		return true;
	}
	UE_LOG(LogItemSystem,Warning,TEXT("%s player does not have inventory to create transfer widget"))
	return false;
	
}

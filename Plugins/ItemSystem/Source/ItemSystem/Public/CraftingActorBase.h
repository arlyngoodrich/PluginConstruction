// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StationCraftingComponent.h"
#include "StorageActorBase.h"
#include "CraftingActorBase.generated.h"

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API ACraftingActorBase : public AStorageActorBase
{
	GENERATED_BODY()
	
	public:
	
	ACraftingActorBase();

	UPROPERTY(VisibleAnywhere)
	UStationCraftingComponent* CraftingComponent;
	
	protected:

	virtual bool CreateStorageWidget(APlayerController* InstigatingPlayer, UCustomUserWidget*& OutWidget) override;
	
	
};

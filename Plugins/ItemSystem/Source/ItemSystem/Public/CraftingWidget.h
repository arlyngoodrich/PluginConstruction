// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "CraftingWidget.generated.h"

/**
 * 
 */

class UCraftingComponent;
class UCraftingRecipeWidget;

UCLASS()
class ITEMSYSTEM_API UCraftingWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly,Category="Crafting Data", meta = (ExposeOnSpawn))
	UCraftingComponent* MyCraftingComponent;
	
	
};

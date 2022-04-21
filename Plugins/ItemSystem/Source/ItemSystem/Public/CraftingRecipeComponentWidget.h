// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "CraftingData.h"
#include "CraftingRecipeComponentWidget.generated.h"

class UCraftingComponent;

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UCraftingRecipeComponentWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly,Category="Crafting Data")
	FRecipeComponent MyRecipeComponent;

	UPROPERTY(BlueprintReadOnly, Category="Crafting Data")
	UCraftingComponent* MyCraftingComponent;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "CraftingData.h"
#include "CraftingRecipeWidget.generated.h"

class UCraftingComponent;

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UCraftingRecipeWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category="Crafting Data")
	FCraftingRecipe MyCraftingRecipe;

	UPROPERTY(BlueprintReadOnly, Category="Crafting Data")
	UCraftingComponent* MyCraftingComponent;
	
};

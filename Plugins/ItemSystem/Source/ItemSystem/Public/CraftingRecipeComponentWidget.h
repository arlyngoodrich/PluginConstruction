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

	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	FRecipeComponent MyRecipeComponent;

	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	UCraftingComponent* MyCraftingComponent;

	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetReferences(FRecipeComponent SetMyRecipeComponent,UCraftingComponent* SetMyCraftingComponent,APlayerController* OwningPlayer);

	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	FItemData ComponentItemData;

	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	int32 ItemQtyInInventories;
	
	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	int32 ItemQtyNeeded;




};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "CraftingData.h"
#include "CraftingRecipeWidget.generated.h"

class UCraftingComponent;
class UCraftingWidget;

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UCraftingRecipeWidget : public UCustomUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	FCraftingRecipe MyCraftingRecipe;

	UPROPERTY(BlueprintReadOnly, Category="Crafting")
	UCraftingComponent* MyCraftingComponent;

	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	UCraftingWidget* MyCraftingWidget;
	
	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetReferences(FCraftingRecipe SetMyCraftingRecipe, UCraftingComponent* SetMyCraftingComponent,
	                   UCraftingWidget* SetMyCraftingWidget, APlayerController* OwningPlayer);

protected:
	
	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	FItemData OutputItemData;

	UPROPERTY(BlueprintReadOnly,Category="Crafting")
	TArray<FItemData> InputItemData;


	UFUNCTION(BlueprintCallable,Category="Crafting")
	void SetActiveRecipe();
	
	void SetItemDataReferences(FCraftingRecipe CraftingRecipe);

	//FItemData ConvertItemBaseToItemData(TSubclassOf<AItemBase> ItemClass) const;
	
};

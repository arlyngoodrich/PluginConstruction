// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingRecipeWidget.h"

void UCraftingRecipeWidget::SetReferences(FCraftingRecipe SetMyCraftingRecipe,
	UCraftingComponent* SetMyCraftingComponent, APlayerController* OwningPlayer)
{
	MyCraftingRecipe = SetMyCraftingRecipe;
	MyCraftingComponent = SetMyCraftingComponent;
	SetOwningPlayer(OwningPlayer);

	SetItemDataReferences(MyCraftingRecipe);
}

void UCraftingRecipeWidget::SetItemDataReferences(const FCraftingRecipe CraftingRecipe)
{
	OutputItemData = ConvertItemBaseToItemData(CraftingRecipe.RecipeOutputs.ComponentClass);

	for (int i = 0; i < CraftingRecipe.RecipeInputs.Num(); ++i)
	{
		InputItemData.Add(ConvertItemBaseToItemData(CraftingRecipe.RecipeInputs[i].ComponentClass));
	}
	
}

FItemData UCraftingRecipeWidget::ConvertItemBaseToItemData(const TSubclassOf<AItemBase> ItemClass) const
{
	if(const AItemBase* ItemBase = Cast<AItemBase>(ItemClass->GetDefaultObject()))
	{
		return ItemBase->GetItemData();
	}
	else
	{
		return FItemData();
	}
	
	}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingRecipeWidget.h"
#include "CraftingWidget.h"
#include "CraftingData.h"
#include "ItemSystem.h"

void UCraftingRecipeWidget::SetReferences(FCraftingRecipe SetMyCraftingRecipe,
                                          UCraftingComponent* SetMyCraftingComponent,UCraftingWidget* SetMyCraftingWidget, APlayerController* OwningPlayer)
{
	MyCraftingRecipe = SetMyCraftingRecipe;
	MyCraftingComponent = SetMyCraftingComponent;
	MyCraftingWidget = SetMyCraftingWidget;
	SetOwningPlayer(OwningPlayer);

	SetItemDataReferences(MyCraftingRecipe);
}


void UCraftingRecipeWidget::SetActiveRecipe()
{
	if(MyCraftingWidget)
	{
		MyCraftingWidget->SetActiveRecipe(MyCraftingRecipe);
	}

}

void UCraftingRecipeWidget::SetItemDataReferences(const FCraftingRecipe CraftingRecipe)
{
	OutputItemData = FRecipeComponent::ConvertComponentToItemData(CraftingRecipe.RecipeOutputs);

	for (int i = 0; i < CraftingRecipe.RecipeInputs.Num(); ++i)
	{
		InputItemData.Add(FRecipeComponent::ConvertComponentToItemData(CraftingRecipe.RecipeInputs[i]));
	}
	
}



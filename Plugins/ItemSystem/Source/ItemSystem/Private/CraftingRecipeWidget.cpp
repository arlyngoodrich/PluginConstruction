// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingRecipeWidget.h"

void UCraftingRecipeWidget::SetReferences(FCraftingRecipe SetMyCraftingRecipe,
	UCraftingComponent* SetMyCraftingComponent, APlayerController* OwningPlayer)
{
	MyCraftingRecipe = SetMyCraftingRecipe;
	MyCraftingComponent = SetMyCraftingComponent;
	SetOwningPlayer(OwningPlayer);
}

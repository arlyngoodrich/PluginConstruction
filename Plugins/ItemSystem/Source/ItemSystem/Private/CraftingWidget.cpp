// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingWidget.h"

#include "CraftingComponent.h"
#include "CraftingRecipeWidget.h"
#include "ItemSystem.h"

UCraftingWidget::UCraftingWidget()
{
	CraftingRecipeWidgetClass = UCraftingRecipeWidget::StaticClass();
	ActiveRecipe = FCraftingRecipe();
}

void UCraftingWidget::SetReferences(UCraftingComponent* SetMyCraftingComponent, APlayerController* OwningPlayer)
{
	MyCraftingComponent = SetMyCraftingComponent;
	SetOwningPlayer(OwningPlayer);
}


void UCraftingWidget::InitializeRecipes()
{
	if(MyCraftingComponent == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("%s crafting UI does not have crafting component set"),
			*GetOwningPlayer()->GetName())
		return;
	}

	CraftingRecipeWidgets.Empty();
	
	const TArray<FCraftingRecipe> CraftingRecipes = MyCraftingComponent->GetEligibleCraftingRecipes();

	for (int i = 0; i < CraftingRecipes.Num(); ++i)
	{
		UCraftingRecipeWidget* NewRecipeWidget = Cast<UCraftingRecipeWidget>(
			CreateWidget(GetOwningPlayer(), CraftingRecipeWidgetClass));

		NewRecipeWidget->SetReferences(CraftingRecipes[i],MyCraftingComponent,this,GetOwningPlayer());
		CraftingRecipeWidgets.Add(NewRecipeWidget);
	}

	BP_SetupCraftingGrid();
	
}

void UCraftingWidget::SetActiveRecipe(const FCraftingRecipe NewActiveRecipe)
{
	ActiveRecipe = NewActiveRecipe;
	bHasActiveRecipe = true;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingWidget.h"

#include "CraftingComponent.h"
#include "CraftingRecipeComponentWidget.h"
#include "CraftingRecipeWidget.h"
#include "ItemSystem.h"

UCraftingWidget::UCraftingWidget()
{
	CraftingRecipeWidgetClass = UCraftingRecipeWidget::StaticClass();
	CraftingRecipeComponentWidgetClass = UCraftingRecipeComponentWidget::StaticClass();
	ActiveRecipe = FCraftingRecipe();
}

void UCraftingWidget::SetReferences(UCraftingComponent* SetMyCraftingComponent, APlayerController* OwningPlayer)
{
	MyCraftingComponent = SetMyCraftingComponent;
	SetOwningPlayer(OwningPlayer);
}

void UCraftingWidget::UpdateCraftingInputComponentQuantities()
{
	if(MyCraftingComponent==nullptr){return;}
	
	for (int i = 0; i < RecipeInputWidgets.Num(); ++i)
	{
		const int32 Qty = MyCraftingComponent->GetAvailableQtyOfItem(RecipeInputWidgets[i]->ComponentItemData);
		RecipeInputWidgets[i]->ItemQtyInInventories = Qty;
	}
}


void UCraftingWidget::InitializeCraftingWidget()
{
	if(MyCraftingComponent == nullptr)
	{
		UE_LOG(LogItemSystem,Error,TEXT("%s crafting UI does not have crafting component set"),
			*GetOwningPlayer()->GetName())
		return;
	}
	
	RefreshRecipeWidgetReferences();
	BP_UpdateRecipeGrid();
}

void UCraftingWidget::SetActiveRecipe(const FCraftingRecipe NewActiveRecipe)
{
	ActiveRecipe = NewActiveRecipe;
	bHasActiveRecipe = true;

	RefreshRecipeInputWidgetReferences();
}

void UCraftingWidget::RefreshRecipeWidgetReferences()
{
	CraftingRecipeWidgets.Empty();
	
	const TArray<FCraftingRecipe> CraftingRecipes = MyCraftingComponent->GetEligibleCraftingRecipes();

	for (int i = 0; i < CraftingRecipes.Num(); ++i)
	{
		UCraftingRecipeWidget* NewRecipeWidget = Cast<UCraftingRecipeWidget>(
			CreateWidget(GetOwningPlayer(), CraftingRecipeWidgetClass));

		NewRecipeWidget->SetReferences(CraftingRecipes[i],MyCraftingComponent,this,GetOwningPlayer());
		CraftingRecipeWidgets.Add(NewRecipeWidget);
	}
}


void UCraftingWidget::RefreshRecipeInputWidgetReferences()
{
	if(bHasActiveRecipe == false){return;}

	//Remove current recipe inputs from parent
	for (int i = 0; i < RecipeInputWidgets.Num(); ++i)
	{
		if(RecipeInputWidgets[i])
		{
			RecipeInputWidgets[i]->RemoveFromParent();
		}
	}

	//remove from array reference
	RecipeInputWidgets.Empty();

	//create and add new ones
	const TArray<FRecipeComponent> NewInputs = ActiveRecipe.RecipeInputs;
	for (int i = 0; i < NewInputs.Num(); ++i)
	{
		UCraftingRecipeComponentWidget* NewInput = Cast<UCraftingRecipeComponentWidget>(CreateWidget(GetOwningPlayer(),
			CraftingRecipeComponentWidgetClass));

		NewInput->SetReferences(NewInputs[i],MyCraftingComponent,GetOwningPlayer());
		RecipeInputWidgets.Add(NewInput);
	}

	UpdateCraftingInputComponentQuantities();
}


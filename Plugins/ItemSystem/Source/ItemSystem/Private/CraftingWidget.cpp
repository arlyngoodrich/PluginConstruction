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

FItemData UCraftingWidget::GetActiveRecipeAsItemData() const {return FRecipeComponent::ConvertComponentToItemData(ActiveRecipe.RecipeOutputs);}


void UCraftingWidget::BeginDestroy()
{
	if(MyCraftingComponent)
	{
		MyCraftingComponent->OnCraftingStarted.RemoveDynamic(this,&UCraftingWidget::OnNewRecipeCraftStart);
		MyCraftingComponent->OnCraftingQueueUpdated.RemoveDynamic(this,&UCraftingWidget::OnCraftingQueueUpdated);
	}
	
	Super::BeginDestroy();
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

	MyCraftingComponent->OnCraftingStarted.AddDynamic(this,&UCraftingWidget::OnNewRecipeCraftStart);
}

void UCraftingWidget::SetReferences(UCraftingComponent* SetMyCraftingComponent, APlayerController* OwningPlayer)
{
	MyCraftingComponent = SetMyCraftingComponent;
	MyCraftingComponent->CraftingUIUpdate.AddDynamic(this,&UCraftingWidget::OnInventoryUpdate);
	MyCraftingComponent->OnCraftingQueueUpdated.AddDynamic(this,&UCraftingWidget::OnCraftingQueueUpdated);
	SetOwningPlayer(OwningPlayer);
}

void UCraftingWidget::OnInventoryUpdate()
{
	UpdateIfRecipesCanBeCrafted();
	UpdateCraftingInputComponentQuantities();
}

void UCraftingWidget::OnCraftingQueueUpdated(TArray<FCraftingQueueSlot> UpdatedQueue)
{
	BP_UpdateCraftingQueue(UpdatedQueue);
}

void UCraftingWidget::UpdateIfRecipesCanBeCrafted()
{
	for (int i = 0; i < CraftingRecipeWidgets.Num(); ++i)
	{
		UCraftingRecipeWidget* TargetWidget = CraftingRecipeWidgets[i];
		const bool bCanRecipeBeCrafted = MyCraftingComponent->CanRecipeBeCrafted(TargetWidget->MyCraftingRecipe);
		TargetWidget->SetIfRecipeCanBeCrafted(bCanRecipeBeCrafted);
	}
}

void UCraftingWidget::UpdateCraftingInputComponentQuantities()
{
	if(MyCraftingComponent==nullptr){return;}
	
	for (int i = 0; i < RecipeInputWidgets.Num(); ++i)
	{
		const int32 Qty = MyCraftingComponent->GetAvailableQtyOfItem(RecipeInputWidgets[i]->GetComponentItemData());
		RecipeInputWidgets[i]->SetInventoryQuantities(Qty);
	}
}



void UCraftingWidget::SetActiveRecipe(const FCraftingRecipe NewActiveRecipe)
{
	ActiveRecipe = NewActiveRecipe;
	bHasActiveRecipe = true;

	RefreshRecipeInputWidgetReferences();
}

void UCraftingWidget::ClearActiveRecipe()
{
	ActiveRecipe = FCraftingRecipe();
	bHasActiveRecipe = false;
	ClearRecipeInputs();
}

void UCraftingWidget::OnNewRecipeCraftStart(const float CraftDuration,const FCraftingRecipe Recipe)
{
	
	bIsRecipeBeingCrafted = true;
	CurrentlyCraftingRecipe = Recipe;

	if(CraftDuration > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CraftingTimer,this,&UCraftingWidget::OnRecipeCraftFinish,CraftDuration,false);
	}
	else
	{
		OnRecipeCraftFinish();
	}
	
	
}

void UCraftingWidget::OnRecipeCraftFinish()
{
	bIsRecipeBeingCrafted = false;
	CurrentlyCraftingRecipe = FCraftingRecipe();
	GetWorld()->GetTimerManager().ClearTimer(CraftingTimer);
}

void UCraftingWidget::RefreshRecipeWidgetReferences()
{
	for (int i = 0; i < CraftingRecipeWidgets.Num(); ++i)
	{
		CraftingRecipeWidgets[i]->RemoveFromParent();
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
	
	UpdateIfRecipesCanBeCrafted();
}


void UCraftingWidget::ClearRecipeInputs()
{
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
}

void UCraftingWidget::RefreshRecipeInputWidgetReferences()
{
	if(bHasActiveRecipe == false){return;}

	ClearRecipeInputs();

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
	BP_UpdateActiveRecipeDetails();
}


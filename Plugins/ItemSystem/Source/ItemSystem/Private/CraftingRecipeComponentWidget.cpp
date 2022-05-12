// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingRecipeComponentWidget.h"

FRecipeComponent UCraftingRecipeComponentWidget::GetRecipeComponent() const { return MyRecipeComponent;}
FItemData UCraftingRecipeComponentWidget::GetComponentItemData() const { return ComponentItemData;}


void UCraftingRecipeComponentWidget::SetReferences(const FRecipeComponent SetMyRecipeComponent,
                                                   UCraftingComponent* SetMyCraftingComponent,APlayerController* OwningPlayer)
{
	MyRecipeComponent = SetMyRecipeComponent;
	MyCraftingComponent = SetMyCraftingComponent;
	SetOwningPlayer(OwningPlayer);
	
	ComponentItemData = FRecipeComponent::ConvertComponentToItemData(MyRecipeComponent);
	SetInputQuantityNeeded(1);
	UpdateAppearance();
}

void UCraftingRecipeComponentWidget::SetInventoryQuantities(int32 SetItemQtyInInventories)
{
	ItemQtyInInventories = SetItemQtyInInventories;
	bHasEnoughQuantity = ItemQtyInInventories >= ItemQtyNeeded;
	UpdateAppearance();
}

void UCraftingRecipeComponentWidget::SetInputQuantityNeeded(const int32 CraftingQuantity)
{
	ItemQtyNeeded = MyRecipeComponent.Quantity * CraftingQuantity;
	bHasEnoughQuantity = ItemQtyInInventories >= ItemQtyNeeded;
	UpdateAppearance();
}


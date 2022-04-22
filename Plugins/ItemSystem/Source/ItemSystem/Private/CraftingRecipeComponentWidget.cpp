// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingRecipeComponentWidget.h"

void UCraftingRecipeComponentWidget::SetReferences(const FRecipeComponent SetMyRecipeComponent,
                                                   UCraftingComponent* SetMyCraftingComponent,APlayerController* OwningPlayer)
{
	MyRecipeComponent = SetMyRecipeComponent;
	MyCraftingComponent = SetMyCraftingComponent;
	SetOwningPlayer(OwningPlayer);
	
	ComponentItemData = FRecipeComponent::ConvertComponentToItemData(MyRecipeComponent);
	ItemQtyNeeded = MyRecipeComponent.Quantity;
}

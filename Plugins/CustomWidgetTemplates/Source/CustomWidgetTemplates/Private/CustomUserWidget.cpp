// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUserWidget.h"

void UCustomUserWidget::RemoveFromParent()
{
	OnRemoveFromParent.Broadcast();
	Super::RemoveFromParent();
}

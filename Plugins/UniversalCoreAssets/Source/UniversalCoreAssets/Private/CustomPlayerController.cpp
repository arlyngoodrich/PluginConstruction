// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "UniversalCoreAssets.h"

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindKey(EKeys::LeftMouseButton,IE_Pressed,this,&ACustomPlayerController::LMBPressed);
	InputComponent->BindKey(EKeys::RightMouseButton,IE_Pressed,this,&ACustomPlayerController::RMBPressed);
	InputComponent->BindKey(EKeys::MouseScrollUp,IE_Pressed,this,&ACustomPlayerController::MouseScrollUp);
	InputComponent->BindKey(EKeys::MouseScrollDown,IE_Pressed,this,&ACustomPlayerController::MouseScrollDown);
	
}

void ACustomPlayerController::LMBPressed()
{
	OnLMBPressed.Broadcast();
}

void ACustomPlayerController::RMBPressed()
{
	OnRMBPressed.Broadcast();
}

void ACustomPlayerController::MouseScrollUp()
{
	OnMouseScrollUp.Broadcast();
}

void ACustomPlayerController::MouseScrollDown()
{
	OnMouseScrollDown.Broadcast();
}

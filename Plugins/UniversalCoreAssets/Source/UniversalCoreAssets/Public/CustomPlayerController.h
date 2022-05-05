// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLMBPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRMBPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseScrollUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseScrollDown);


/**
 * 
 */
UCLASS()
class UNIVERSALCOREASSETS_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void SetupInputComponent() override;

	UPROPERTY()
	FOnLMBPressed OnLMBPressed;

	UPROPERTY()
	FOnRMBPressed OnRMBPressed;

	UPROPERTY()
	FOnMouseScrollUp OnMouseScrollUp;

	UPROPERTY()
	FOnMouseScrollDown OnMouseScrollDown;


protected:

	virtual void LMBPressed();

	virtual void RMBPressed();

	virtual void MouseScrollUp();

	virtual void MouseScrollDown();

};

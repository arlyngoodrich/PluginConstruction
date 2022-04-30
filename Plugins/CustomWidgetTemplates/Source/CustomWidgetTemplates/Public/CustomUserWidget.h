// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomUserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemoveFromParent);

/**
 * 
 */
UCLASS()
class CUSTOMWIDGETTEMPLATES_API UCustomUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FOnRemoveFromParent OnRemoveFromParent;

	virtual void RemoveFromParent() override;
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CustomUserWidget.h"
#include "UIPlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType,Blueprintable)
class CUSTOMWIDGETTEMPLATES_API UUIPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CUSTOMWIDGETTEMPLATES_API IUIPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,Category="UI Interface")
	void OpenUI(UCustomUserWidget* Widget);

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,Category="UI Interface")
	void CloseUI(UCustomUserWidget* Widget);
	
};

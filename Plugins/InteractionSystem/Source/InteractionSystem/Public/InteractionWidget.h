// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * @breif Native class for widget to be displayed when an object is in focus
 */
UCLASS()
class INTERACTIONSYSTEM_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/**
	 * @brief Text for widget to display
	 */
	UPROPERTY(BlueprintReadOnly, Category="Interaction System")
	FText InteractionText;
	
};

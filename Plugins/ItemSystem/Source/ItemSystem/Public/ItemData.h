// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

class AItemBase;

USTRUCT(BlueprintType)
struct FItemData 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Data")
	FGuid ItemGuid;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* ItemIconSmall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* ItemIconLarge;
	
	UPROPERTY(BlueprintReadOnly, Category = "Item Data")
	TSubclassOf<AItemBase> InWorldClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FVector2D ItemSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float PerItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	bool bShouldItemStack;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Item Data")
	int32 ItemQuantity;

	//Should be between 0 and 1 and be displayed as a percent
	UPROPERTY( BlueprintReadOnly, Category = "Item Data")
	float ItemHealth;
	
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

class AItemBase;

USTRUCT(BlueprintType)
struct FInventory2D 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Position")
	int32 X;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Position")
	int32 Y;

	FInventory2D()
	{
		X = 1;
		Y = 1;
	}

	FInventory2D(const int32 InX, const int32 InY)
	{
		X = InX;
		Y = InY;
	}

	
};


USTRUCT(BlueprintType)
struct FItemData 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Data")
	FGuid ItemGuid;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* ItemIconSmall = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* ItemIconLarge = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Item Data")
	TSubclassOf<AItemBase> InWorldClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FInventory2D ItemSize;

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

USTRUCT(BlueprintType)
struct FInventorySlot 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	FInventory2D Position;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	bool bIsOccupied;	
	
};



USTRUCT(BlueprintType)
struct FInventoryItemData 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	FInventory2D StartPosition;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	FItemData Item;	
	
};
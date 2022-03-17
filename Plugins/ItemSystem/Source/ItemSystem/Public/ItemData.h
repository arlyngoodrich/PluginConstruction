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

	FString GetPositionAsString() const
	{
		FString Position;

		Position.Append("(");
		Position.AppendInt(X);
		Position.Append(",");
		Position.AppendInt(Y);
		Position.Append(")");

		return Position;
	}

	bool IsEqualTo(const FInventory2D In2D) const
	{
		if(In2D.X == X && In2D.Y == Y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	

	bool operator==(const FInventory2D& Position) const
	{
		return (X == Position.X && Y == Position.Y);
	}
};


USTRUCT(BlueprintType)
struct FItemData 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Data")
	FGuid ItemGuid;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* ItemIconSmall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* ItemIconLarge;
	
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

	FItemData() 
	{
		DisplayName = "Default";
		ItemGuid.Invalidate();
		ItemIconSmall = nullptr;
		ItemIconLarge = nullptr;
		InWorldClass = nullptr;
		ItemSize = FInventory2D(1,1);
		ItemHealth = 1; 
		ItemQuantity = 1;
		bShouldItemStack = false;
		PerItemWeight=  1;
	}

	static FItemData NewItem(const FName NewDisplayName, const TSubclassOf<AItemBase> NewItemClass)
	{
		FItemData ItemData = FItemData();
		ItemData.DisplayName = NewDisplayName;
		ItemData.ItemGuid = FGuid::NewGuid();
		ItemData.InWorldClass = NewItemClass;

		return ItemData;
	}

	static FItemData NewItem(const FName NewDisplayName, const TSubclassOf<AItemBase> NewItemClass,
	                         const FInventory2D NewItemSize, const int32 NewItemQuantity, const bool bNewItemStacks,
	                         const float NewItemPerWeight)
	{
		FItemData ItemData = FItemData();
		
		ItemData.DisplayName = NewDisplayName;
		ItemData.ItemGuid = FGuid::NewGuid();
		ItemData.InWorldClass = NewItemClass;
		ItemData.ItemSize = NewItemSize;
		ItemData.ItemQuantity = NewItemQuantity;
		ItemData.bShouldItemStack = bNewItemStacks;
		ItemData.PerItemWeight = NewItemPerWeight;

		return ItemData;
	}
	
	float GetStackWeight() const
	{
		return PerItemWeight * ItemQuantity;
	}

	bool IsEqualTo(const FItemData InItem) const
	{
		return ItemGuid == InItem.ItemGuid;
	}

	bool operator==(const FItemData& Item) const
	{
		return ItemGuid == Item.ItemGuid;
	}
	
};






// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "InventoryData.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Data")
	FInventory2D Position;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	bool bIsOccupied;


	FInventorySlot()
	{
		Position = FInventory2D(0,0);
		bIsOccupied = false;
	}

	FInventorySlot(const FInventory2D InPosition, const bool InbIsOccupied)
	{
		Position = InPosition;
		bIsOccupied = InbIsOccupied;
	}

	bool operator==(const FInventorySlot& InventorySlot) const
	{
		return Position.IsEqualTo(InventorySlot.Position);
	}
	
	
};



USTRUCT(BlueprintType)
struct FInventoryItemData 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	FInventory2D StartPosition;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	FItemData Item;

	FInventoryItemData()
	{
		StartPosition = FInventory2D();
		Item = FItemData();
	}

	FInventoryItemData(const FInventory2D InPosition, const FItemData InItem)
	{
		StartPosition = InPosition;
		Item = InItem;
	}


	TArray<FInventory2D> GetCoveredSlots() const
	{
		TArray<FInventory2D> CoveredSlots;

		for (int x = 0; x < Item.ItemSize.X; ++x)
		{
			for (int y = 0; y < Item.ItemSize.Y; ++y)
			{
				const int32 XPos = x + StartPosition.X;
				const int32 YPos = y + StartPosition.Y;
				FInventory2D CoveredSlotPos = FInventory2D(XPos,YPos);
				CoveredSlots.Add(CoveredSlotPos);
			}
		}
		return  CoveredSlots;
	}

	void RotateItem()
	{
		Item.Rotate();
	}

	bool operator==(const FInventoryItemData& InventoryItemData) const
	{
		return Item.IsEqualTo(InventoryItemData.Item) && StartPosition == InventoryItemData.StartPosition;
	}  
	
};

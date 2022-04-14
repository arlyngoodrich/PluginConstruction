// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "InventoryData.generated.h"

/**
 * @brief Data Struct for Inventory Slots
 */
USTRUCT(BlueprintType)
struct FInventorySlot 
{
	GENERATED_USTRUCT_BODY()

	/**
	 * @brief 2D Position of slot in inventory
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Data")
	FInventory2D Position;

	/**
	 * @brief If the slot is occupied or not
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	bool bIsOccupied;

	/**
	 * @brief If struct has been properly constructed
	 */
	bool bIsValid;


	/**
	 * @brief Default constructor. Sets to invalid position and occupied to false
	 */
	FInventorySlot()
	{
		Position = FInventory2D();
		bIsOccupied = false;
		bIsValid = false;
	}

	
	/**
	 * @brief Creates new inventory slot with given position and occupied vale
	 * @param InPosition Position of slot
	 * @param InbIsOccupied If slot is occupied or not
	 */
	FInventorySlot(const FInventory2D InPosition, const bool InbIsOccupied)
	{
		Position = InPosition;
		bIsOccupied = InbIsOccupied;
		bIsValid = true;
	}

	bool operator==(const FInventorySlot& InventorySlot) const
	{
		return Position.IsEqualTo(InventorySlot.Position);
	}
	
	
};


/**
 * @brief Data Struct for Item Data and it's position in an inventory
 */
USTRUCT(BlueprintType)
struct FInventoryItemData 
{
	GENERATED_USTRUCT_BODY()

	/**
	 * @brief Item Position
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	FInventory2D StartPosition;

	/**
	 * @brief Item Data
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	FItemData Item;

	/**
	 * @brief Constructs null item and start position 
	 */
	FInventoryItemData()
	{
		StartPosition = FInventory2D();
		Item = FItemData();
	}

	/**
	 * @brief Valid constructor for Inventory Data
	 * @param InPosition Position for item data
	 * @param InItem Item data
	 */
	FInventoryItemData(const FInventory2D InPosition, const FItemData InItem)
	{
		StartPosition = InPosition;
		Item = InItem;
	}


	/**
	 * @brief Used to get slots that are covered by inventory item given it's position
	 * @return Array of slot positions that would be covered given it's position
	 */
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

	/**
	 * @brief Rotates Inventory Item
	 */
	void RotateItem()
	{
		Item.Rotate();
	}

	bool operator==(const FInventoryItemData& InventoryItemData) const
	{
		return Item.IsEqualTo(InventoryItemData.Item) && StartPosition == InventoryItemData.StartPosition;
	}  
	
};

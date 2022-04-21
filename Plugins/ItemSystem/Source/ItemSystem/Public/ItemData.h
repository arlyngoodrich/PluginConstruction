// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

class AItemBase;

/**
 * @brief 2D Item Struct for Inventory Size and Position 
 */
USTRUCT(BlueprintType)
struct FInventory2D 
{
	GENERATED_USTRUCT_BODY()

	/**
	 * @brief X Position 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Position")
	int32 X;

	/**
	 * @brief Y Position
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Position")
	int32 Y;

	bool bIsValid;

	/**
	 * @brief Default Constructor for FInventory2D. Is an invalid position.  
	 */
	FInventory2D()
	{
		X = 0;
		Y = 0;
		bIsValid = false;
	}

	/**
	 * @brief Valid constructor for FInventory2D.  
	 * @param InX X Position
	 * @param InY Y Position
	 */
	FInventory2D(const int32 InX, const int32 InY)
	{
		X = InX;
		Y = InY;
		bIsValid = true;
	}

	/**
	 * @brief Helper function to convert position in a string for logging
	 * @return Positions as string
	 */
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

	/**
	 * @brief Checks to see if two positions are equal and valid
	 * @param In2D Position to check
	 * @return returns true if equal, false if not
	 */
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


/**
 * @brief Data struct for Item Data
 */
USTRUCT(BlueprintType)
struct FItemData 
{
	GENERATED_USTRUCT_BODY()

	/**
	 * @brief Item GUID used for tracking unique item stacks
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Item Data")
	FGuid ItemGuid;
	
	/**
	 * @brief Item Display name.  Purely cosmetic. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName DisplayName;

	/**
	 * @brief Small icon used for displaying while moving or in condensed menus
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* ItemIconSmall;

	/**
	 * @brief Large icon for displaying item in inventory grid.  
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* ItemIconLarge;

	/**
	 * @brief In world item class of item 
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Item Data")
	TSubclassOf<AItemBase> InWorldClass;

	/**
	 * @brief 2D size of item 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FInventory2D ItemSize;

	/**
	 * @brief Weight of a single item
	 */	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float PerItemWeight;

	/**
	 * @brief If item should attempted to be stacked 
	 */	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	bool bShouldItemStack;

	/**
	 * @brief Item stack quantity
	*/	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Item Data")
	int32 ItemQuantity;

	/**
	* @brief Max stack quantity 
	*/	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Item Data")
	int32 MaxStackQuantity;
	
	/**
	 * @brief Should be between 0 and 1 and be displayed as a percent
	 */
	UPROPERTY( BlueprintReadOnly, Category = "Item Data")
	float ItemHealth;

	/**
	 * @brief If an item is rotated from it's original orientation 
	 */
	UPROPERTY( BlueprintReadOnly, Category = "Item Data")
	bool bIsRotated;

	
	/**
	 * @brief If item is valid
	 */
	UPROPERTY( BlueprintReadOnly, Category = "Item Data")
	bool bIsValid;

	/**
	 * @brief Default constructor for item.  Item is invalid.
	 */
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
		MaxStackQuantity = 1;
		bShouldItemStack = false;
		PerItemWeight=  1;
		bIsRotated = false;
		bIsValid = false;
	}

	/**
	 * @brief Valid Constructor for new items
	 */
	static FItemData NewItem(const FName NewDisplayName, const TSubclassOf<AItemBase> NewItemClass)
	{
		FItemData ItemData = FItemData();
		ItemData.DisplayName = NewDisplayName;
		ItemData.ItemGuid = FGuid::NewGuid();
		ItemData.InWorldClass = NewItemClass;
		ItemData.bIsValid = true;

		return ItemData;
	}

	/**
	 * @brief Valid Constructor for new items
	 */
	static FItemData NewItem(const FName NewDisplayName, const TSubclassOf<AItemBase> NewItemClass,
						 const FInventory2D NewItemSize, const float NewItemPerWeight)
	{
		FItemData ItemData = FItemData();
		
		ItemData.DisplayName = NewDisplayName;
		ItemData.ItemGuid = FGuid::NewGuid();
		ItemData.InWorldClass = NewItemClass;
		ItemData.ItemSize = NewItemSize;
		ItemData.PerItemWeight = NewItemPerWeight;
		ItemData.bIsValid = true;
		return ItemData;
	}

	/**
	 * @brief Valid Constructor for new items
	 */
	static FItemData NewItem(const FName NewDisplayName, const TSubclassOf<AItemBase> NewItemClass,
	                         const FInventory2D NewItemSize, const int32 NewItemQuantity, const int32 NewMaxStack,
	                         const bool bNewItemStacks, const float NewItemPerWeight)
	{
		FItemData ItemData = FItemData();
		
		ItemData.DisplayName = NewDisplayName;
		ItemData.ItemGuid = FGuid::NewGuid();
		ItemData.InWorldClass = NewItemClass;
		ItemData.ItemSize = NewItemSize;
		ItemData.ItemQuantity = NewItemQuantity;
		ItemData.MaxStackQuantity = NewMaxStack;
		ItemData.bShouldItemStack = bNewItemStacks;
		ItemData.PerItemWeight = NewItemPerWeight;
		ItemData.bIsValid = true;
		return ItemData;
	}

	
	/**
	 * @brief Rotates item's orientation 
	 */
	void Rotate()
	{
		const int32 NewXSize = ItemSize.Y;
		const int32 NewYSize = ItemSize.X;
		const FInventory2D NewSize = FInventory2D(NewXSize,NewYSize);

		const bool bOldRotated = bIsRotated;

		bIsRotated = !bOldRotated;
		ItemSize = NewSize;
	}

	/**
	 * @brief Invalidates item
	 */
	void Invalidate()
	{
		ItemGuid.Invalidate();
		InWorldClass = nullptr;
		ItemQuantity = 0;
		bIsValid = false;
	}
	
	
	/**
	 * @return Total weight of item stack
	 */
	float GetStackWeight() const
	{
		return PerItemWeight * ItemQuantity;
	}


	/**
	 * @brief Checks to see if items are equal
	 * @param InItem Item to check
	 * @return true if equal, false if not
	 */
	bool IsEqualTo(const FItemData InItem) const
	{
		return ItemGuid == InItem.ItemGuid && InItem.bIsValid;
	}

	bool operator==(const FItemData& Item) const
	{
		return ItemGuid == Item.ItemGuid && Item.bIsValid;
	}
	
};






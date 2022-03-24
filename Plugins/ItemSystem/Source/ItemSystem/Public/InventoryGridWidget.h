// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "ItemData.h"
#include "InventoryGridWidget.generated.h"

class UInventoryComponent;
class UInventorySlotWidget;
class UInventoryItemWidget;

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UInventoryGridWidget : public UCustomUserWidget
{
	GENERATED_BODY()
	
public:

	UInventoryGridWidget();

	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn),Category="Inventory")
	UInventoryComponent* OwningInventoryComponent;

	void SetSlotsOnDragOver(FInventory2D DragPosition, FItemData DraggedItem);

	void OnItemDragStart(const UInventoryItemWidget* InventoryItemWidget);

	void OnItemDragCancel(const UInventoryItemWidget* InventoryItemWidget);


protected:
	
	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass;
	
	UPROPERTY(BlueprintReadWrite,Category="Inventory")
	TArray<UInventorySlotWidget*> SlotWidgets;

	UPROPERTY(BlueprintReadWrite,Category="Inventory")
	TArray<UInventoryItemWidget*> ItemWidgets;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	//This function should be implemented in blueprint to add the new slot widgets as children in a uniform
	//grid panel.  Should only be called on initial setup as inventory grids should not change.  
	UFUNCTION(BlueprintImplementableEvent,Category="Inventory",meta = (DisplayName = "Set Slot Widgets in Grid"))
	void BP_SetSlotWidgetsInGrid();

	//This function should be implemented in blueprint to set the items to their target positions.  Should be called for
	//all updates to inventory as item positions, rotation, and quantities may change.  
	UFUNCTION(BlueprintImplementableEvent,Category="Inventory",meta = (DisplayName = "Set Item Widgets in Grid"))
	void BP_SetItemWidgetsInGrid();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void InitializeInventory();
	
	void InitializeGrid();

	void InitializeItems();

	UFUNCTION()
	void OnInventorySlotUpdate();

	UFUNCTION()
	void OnInventoryItemUpdates();

	void ClearItemWidgets();

	void ClearSlotWidgets();
	
	bool GetSlotWidgetFromPosition(FInventory2D Position, UInventorySlotWidget*& OutSlotWidget);


	
	
};

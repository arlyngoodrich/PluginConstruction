// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUserWidget.h"
#include "InventoryData.h"
#include "ItemData.h"
#include "InventoryGridWidget.generated.h"

class UInventoryComponent;
class UInventorySlotWidget;
class UInventoryItemWidget;

/**
 * @brief Base class of user widget for inventory grids.  Handles back end logic for widget. 
 */
UCLASS()
class ITEMSYSTEM_API UInventoryGridWidget : public UCustomUserWidget
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief Default constructor
	 */
	UInventoryGridWidget();

	/**
	 * @brief Owning Inventory component for inventory grid widget 
	 */
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn),Category="Inventory")
	UInventoryComponent* OwningInventoryComponent;

	/**
	 * @brief Called during drag operation 
	 * @param DragPosition Current Drag position in inventory
	 * @param DraggedItem Item being dragged.  Used to set slots as hovered  
	 */
	void SetSlotsOnDragOver(FInventory2D DragPosition, FItemData DraggedItem);

	/**
	 * @brief Called by Item widget when drag starts
	 * @param InventoryItemWidget Item widget that is starting to get dragged
	 */
	void OnItemDragStart(const UInventoryItemWidget* InventoryItemWidget);

	/**
	 * @brief Called by Item Widget when dragging cancelled -- not on item drop
	 * @param InventoryItemWidget Item widget that has dragging cancelled 
	 */
	void OnItemDragCancel(const UInventoryItemWidget* InventoryItemWidget);
	
	/**
	 * @brief Called when grid is created by a storage inventory component.  Used to facilitate double click on Item Widget
	 * to trigger transfer
	 * @param TargetInventoryComponent 
	 */
	UFUNCTION(BlueprintCallable,Category = "Inventory")
	void AllowTransfer(UInventoryComponent* TargetInventoryComponent);
	
	/**
	 * @brief Used to remove transfer option so Item Widget will not attempt transfer if double clicked.  Should be called
	 * by storage inventory component when storage is is closed.
	 */
	UFUNCTION(BlueprintCallable,Category = "Inventory")
	void RemoveTransfer();

	/**
	 * @brief Called from Item Widget when double clicked
	 * @param TargetItem 
	 */
	UFUNCTION(BlueprintCallable,Category = "Inventory")
	void RequestTransfer(FInventoryItemData TargetItem) const;

	
protected:
	
	/**
	 * @brief Slot widget base class. Set in BP
	 */
	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	/**
	 * @brief Item Widget base class. Set in BP
	 */
	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass;
	
	/**
	 * @brief Reference array of Slot Widgets.  Filled during initialization after inventory owner reference is set and
	 * updated by delegate broadcast from owning inventory. 
	 */
	UPROPERTY(BlueprintReadWrite,Category="Inventory")
	TArray<UInventorySlotWidget*> SlotWidgets;

	/**
	 * @brief Reference array of item widgets.  Filled during initialization after inventory owner reference is set and
	 * updated by delegate broadcast from owning inventory.
	 */
	UPROPERTY(BlueprintReadWrite,Category="Inventory")
	TArray<UInventoryItemWidget*> ItemWidgets;

	/**
	 * @brief Reference to inventory that could be transferred to.  May be null if transfer not available
	 */
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	UInventoryComponent* TransferInventory;

	/**
	 * @brief Boolean for if transfer is available.  
	 */
	UPROPERTY(BlueprintReadOnly,Category="Inventory")
	bool bTransferAvailable;
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	/**
	 * @brief This function should be implemented in blueprint to add the new slot widgets as children in a uniform
	 * grid panel.  Should only be called on initial setup as inventory grids should not change.  
	 */
	UFUNCTION(BlueprintImplementableEvent,Category="Inventory",meta = (DisplayName = "Set Slot Widgets in Grid"))
	void BP_SetSlotWidgetsInGrid();
	
	/**
	 * @brief This function should be implemented in blueprint to set the items to their target positions.  Should be called for
	 * all updates to inventory as item positions, rotation, and quantities may change.  
	 */
	UFUNCTION(BlueprintImplementableEvent,Category="Inventory",meta = (DisplayName = "Set Item Widgets in Grid"))
	void BP_SetItemWidgetsInGrid();

	/**
	 * @brief Triggered by BP on construct to setup up Item Widget and Slot references in native code
	 */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void InitializeInventory();
	
	/**
	 * @brief Sets up grid
	 */
	void InitializeGrid();

	/**
	 * @brief Sets up items
	 */
	void InitializeItems();

	/**
	 * @brief Bound to owning inventory FOnSlotUpdate delegate
	 */
	UFUNCTION()
	void OnInventorySlotUpdate();

	/**
	 * @brief Bound to owning inventory FOnInventoryUpdate delegate
	 */
	UFUNCTION()
	void OnInventoryItemUpdates();

	/**
	 * @brief Removes all references item widgets and delegate binding to owning inventory
	 */
	void ClearItemWidgets();

	/**
	 * @brief Removes all slot widgets and delegate binding to owning inventory
	 */
	void ClearSlotWidgets();
	
	/**
	 * @brief Helper function to get slot widget reference given inventory position
	 * @param Position Target position
	 * @param OutSlotWidget Slot widget reference
	 * @return Will return true if widget found along with slot widget reference, will return false if no matching widget
	 * in position is found
	 */
	bool GetSlotWidgetFromPosition(FInventory2D Position, UInventorySlotWidget*& OutSlotWidget);


	
	
};

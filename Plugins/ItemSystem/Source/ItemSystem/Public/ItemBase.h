// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "ItemBase.generated.h"

/**
 * @brief Base class for any item that can be in an inventory
 */
UCLASS()
class ITEMSYSTEM_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * @brief  Sets default values for this actor's properties
	 */
	AItemBase();

	FItemData GetItemData() const;

protected:
	/**
	 * @brief Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

public:	
	/**
	 * @brief Called every frame
	 * @param DeltaTime Time between frames
	 */
	virtual void Tick(float DeltaTime) override;

protected:

	/**
	 * @brief Item data struct for representing the item in an inventory
	 */
	UPROPERTY(Replicated, EditAnywhere,BlueprintReadOnly,Category="Item Data")
	FItemData ItemData;
	
	/**
	 * @brief Adds GUID and Item in world class to Item Data struct
	 */
	void InitializeItemData();

};

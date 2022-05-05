// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "ItemBase.generated.h"

class UInteractableObjectComponent;

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

	/**
	 * @return Item Data
	 */
	FItemData GetItemData() const;

	/**
	 * @brief Sets item data if authority
	 */
	void SetItemData(FItemData SetItemData);

	/**
	 * @brief Facilitates picking up item and adding to inventory
	 */
	UPROPERTY(VisibleAnywhere,Category="Item")
	UInteractableObjectComponent* InteractableObjectComponent;
	

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

	UFUNCTION(BlueprintNativeEvent,BlueprintAuthorityOnly,Category="Item")
	void OnPlayerInteraction(AActor* InstigatingActor);
	
	/**
	 * @brief Adds GUID and Item in world class to Item Data struct
	 */
	void InitializeItemData();

	/**
	 * @brief Allows for native override of interaction method
	 * @param InstigatingActor Actor instigating the interaction
	 */
	virtual void Native_OnPlayerInteraction(AActor* InstigatingActor);

};

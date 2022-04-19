// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingData.h"
#include "CraftingComponent.generated.h"

class UInventoryComponent;

/**
 * @brief Base component for creating new items from other items
 */
UCLASS( ClassGroup=(Crafting), Blueprintable, meta=(BlueprintSpawnableComponent) )
class ITEMSYSTEM_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * @brief Data table to reference for filling Eligible Crafting Recipe array
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Crafting Data")
	UDataTable* CraftingRecipeTable;

	/**
	 * @brief Eligible Crafting Components that this component can use.
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Crafting Data")
	TArray<FCraftingRecipe> EligibleCraftingRecipes;

	

		
};

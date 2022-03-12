// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryInstance.generated.h"


/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UInventoryInstance : public UObject
{
	GENERATED_BODY()

public:

	void Initialize(FVector2D InventorySize, float MaxWeight,);

	
	
};

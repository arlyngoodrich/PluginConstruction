// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FoliageSwapper.generated.h"

class ACustomFoliageManager;

UCLASS( ClassGroup=(Foliage), meta=(BlueprintSpawnableComponent) )
class ENVIRONMENTSYSTEM_API UFoliageSwapper : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFoliageSwapper();

protected:

	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	ACustomFoliageManager* FoliageManager;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	void CheckInToFoliageManager();

	void CheckOutOfFoliageManager();

	
};

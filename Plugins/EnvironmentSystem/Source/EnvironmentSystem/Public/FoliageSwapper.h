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

	/**
	 * @brief Foliage Manager found in worl
	 */
	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	ACustomFoliageManager* FoliageManager;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * @brief Used to check out from foliage manager
	 */
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	/**
	 * @brief Let the foliage manager know this component should be tracked
	 */
	void CheckInToFoliageManager();

	/**
	* @brief Let the foliage manager know this component should no longer be tracked
	*/
	void CheckOutOfFoliageManager();

	
};

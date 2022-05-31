// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomFoliageManager.generated.h"

class UCustomFoliageISMC;
class UFoliageSwapper;
class ACustomFoliageBase;

UCLASS()
class ENVIRONMENTSYSTEM_API ACustomFoliageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomFoliageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void CheckInComponent(UFoliageSwapper* ComponentToTrack);

	void CheckOutComponent(UFoliageSwapper* ComponentToRemove);

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Foliage System")
	float SwapLoopRate = .1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Foliage System")
	float SwapRange = 500;
	
	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	TArray<UFoliageSwapper*> TrackedComponents;

	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	TArray<UCustomFoliageISMC*> CustomFoliageISMCs;

	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	TArray<ACustomFoliageBase*> TrackedFoliage;
	

	UPROPERTY(BlueprintReadOnly,Category="Foliage System")
	FTimerHandle FoliageSwapTimerHandle;

	bool bSwapFoliage;

	void GetCustomFoliageISMCs();

	void StartFoliageSwapLoop();

	void FoliageSwapLoop();

	void SwapInstancesInRangeWithActors();
	
	
};

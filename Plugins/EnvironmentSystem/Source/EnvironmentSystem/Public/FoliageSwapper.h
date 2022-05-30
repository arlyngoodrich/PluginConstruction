// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FoliageSwapper.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENVIRONMENTSYSTEM_API UFoliageSwapper : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFoliageSwapper();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Foliage System")
	float SwapDistance;

	
		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 * 
 */

class ACustomCharacter;

UCLASS()
class CHARACTERLOCOMOTION_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;

protected:

	UPROPERTY()
	ACustomCharacter* OwningCharacter;

	float DefaultMaxSpeed;
	
};

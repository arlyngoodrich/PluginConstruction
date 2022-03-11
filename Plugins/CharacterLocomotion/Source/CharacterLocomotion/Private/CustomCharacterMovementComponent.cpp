// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"
#include "CustomCharacter.h"


void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACustomCharacter>(PawnOwner);

	DefaultMaxSpeed = OwningCharacter->GetDefaultWalkSpeed();

}

float UCustomCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (OwningCharacter)
	{
		if (OwningCharacter->GetWantsToSprint() && OwningCharacter->bIsCrouched == false)
		{
			MaxSpeed = DefaultMaxSpeed * OwningCharacter->GetSprintSpeedModifier();
		}
		else
		{
			MaxSpeed = DefaultMaxSpeed;
		}
	}

	return MaxSpeed;
}
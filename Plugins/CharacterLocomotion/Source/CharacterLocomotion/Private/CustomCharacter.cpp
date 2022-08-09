// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacter.h"
#include "CharacterLocomotion.h"
#include "CustomCharacterMovementComponent.h"

//UE4 Includes
#include "AbilitySystem/BaseStaminaAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACustomCharacter::ACustomCharacter(const FObjectInitializer& ObjectInitializer)


	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))

{

	// ==== Default Values ===== =====
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// ==== Components ===== =====

	//Add Spring Arm for Camera
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->bEnableCameraRotationLag = true;
	SpringArmComp->bInheritPitch = false;

	//Spring Arm default values
	SpringArmComp->SetRelativeLocation(FVector(20.f, 0.f, 85.f));
	SpringArmComp->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArmComp->TargetArmLength = 150.f;

	//Add Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);


	//Default Movement Values
	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = DefaultCrouchSpeed;

	//Stamina Attribute
	StaminaAttributes = CreateDefaultSubobject<UBaseStaminaAttributeSet>(TEXT("Stamina Attribute"));
	
}

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}




// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// ==== Replicated Variables ===== =====

void ACustomCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACustomCharacter, bWantsToSprint);
	DOREPLIFETIME(ACustomCharacter, bIsSprinting);
	DOREPLIFETIME(ACustomCharacter, SprintSpeedMultiplier);
	
}




// ==== Movement ===== =====


// Called to bind functionality to input
void ACustomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACustomCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACustomCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &ACustomCharacter::TurnRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACustomCharacter::LookUp);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACustomCharacter::Jump);
	PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &ACustomCharacter::ToggleCrouch);

}

bool ACustomCharacter::GetWantsToSprint()
{
	// resets sprint if not moving
	if (GetVelocity().Size() == 0 && bWantsToSprint == true)
	{
		SetWantsToSprint();
	}

	return bWantsToSprint;
}

float ACustomCharacter::GetSprintSpeedModifier() const { return SprintSpeedMultiplier; }

void ACustomCharacter::SetSprintSpeedModifer(const float NewSprintModifer)
{
	if (HasAuthority())
	{
		SprintSpeedMultiplier = NewSprintModifer;
	}

}

float ACustomCharacter::GetDefaultWalkSpeed() const { return DefaultWalkSpeed; }


//Forward and Back
void ACustomCharacter::MoveForward(float Value)
{

	AddMovementInput(GetActorForwardVector() * Value);

}


//Right and Left
void ACustomCharacter::MoveRight(float Value)
{

	AddMovementInput(GetActorRightVector() * Value);

}

void ACustomCharacter::TurnRight(float Value)
{

	AddControllerYawInput(Value);

}


void ACustomCharacter::LookUp(float Value)
{

	if (CameraComp == nullptr) { return; }

	AddControllerPitchInput(Value);


}

void ACustomCharacter::Jump()
{
	Super::Jump();
}

void ACustomCharacter::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
		if (bWantsToSprint == true) { SetWantsToSprint(); }
	}
}



void ACustomCharacter::SetWantsToSprint()
{

	if (GetLocalRole() < ROLE_Authority)
	{
		// Is NOT Authority
		//Server_SetWantsToSprint();
	}
	else
	{
		//Is Authority
		if (bWantsToSprint == false)
		{
			bWantsToSprint = true;
			WantsToSprint_OnSprintStart.Broadcast();
			UE_LOG(LogCharacterLocomotion, Log, TEXT("Character wants to sprint"))

		}
		else
		{
			bWantsToSprint = false;
			WantsToNotSprint_OnSptrintStop.Broadcast();
			UE_LOG(LogCharacterLocomotion, Log, TEXT("Character wants to stop sprint"))
		}
	}
}


bool ACustomCharacter::Server_SetWantsToSprint_Validate()
{
	return true;
}

void ACustomCharacter::Server_SetWantsToSprint_Implementation()
{
	SetWantsToSprint();

}

float ACustomCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage_DamageTaken.Broadcast(DamageAmount);

	return DamageAmount;
}






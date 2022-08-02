// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacter.h"
#include "CharacterLocomotion.h"
#include "CustomCharacterMovementComponent.h"

//UE4 Includes
#include "UniversalData.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/BaseGameplayAbility.h"
#include "UniversalCoreAssets/Public/AbilitySystem/BaseAbilitySystemComponent.h"
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

	//Create Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("Attributes"));
	
	
}

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACustomCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Server GAS init
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
	}
}

void ACustomCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Client GAS init
	SetupAbilitiesInput();
}

UAbilitySystemComponent* ACustomCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACustomCharacter::AddStartUpGameplayAbilities()
{
	check(AbilitySystemComponent)

	if(GetLocalRole() == ROLE_Authority && bAbilitiesInitialized == false)
	{
		//Grant default abilities on server
		for (TSubclassOf<UBaseGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility,
				1,
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
				this));
		}

		//Apply Passive Effects
		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
				GameplayEffect,1,EffectContextHandle);

			if(EffectSpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle =
					AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
						*EffectSpecHandle.Data.Get(),AbilitySystemComponent);
			}
			
		}

		bAbilitiesInitialized = true;
		
	}
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

	SetupAbilitiesInput();

	PlayerInputComponent->BindAxis("MoveForward", this, &ACustomCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACustomCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &ACustomCharacter::TurnRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACustomCharacter::LookUp);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACustomCharacter::Jump);
	PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &ACustomCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("ToggleSprint", IE_Pressed, this, &ACustomCharacter::SetWantsToSprint);

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
		Server_SetWantsToSprint();
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

void ACustomCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, ACustomCharacter* InstigatingCharacter, AActor* DamageCauser)
{
	OnDamage(DamageAmount,HitInfo,DamageTags,InstigatingCharacter,DamageCauser);
}

void ACustomCharacter::HandleHealthChange(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if(bAbilitiesInitialized)
	{
		OnHealthChange(DeltaValue,EventTags);
	}
	
}

void ACustomCharacter::SetupAbilitiesInput()
{
	if(AbilitySystemComponent && InputComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"EUniversalAbilityInputID",
			static_cast<int32>(EUniversalAbilityInputID::Confirm),
			static_cast<int32>(EUniversalAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent,Binds);
	}
	else
	{
		UE_LOG(LogAbilitySystem,Error,TEXT("%s failed to setup inputs for ability system"),*GetName())
	}
}

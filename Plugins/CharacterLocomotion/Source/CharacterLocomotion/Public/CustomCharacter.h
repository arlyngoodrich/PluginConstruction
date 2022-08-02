// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "CustomCharacter.generated.h"

class UBaseGameplayAbility;
class UBaseAbilitySystemComponent;
class UBaseAttributeSet;
class UCameraComponent;
class USpringArmComponent;

//Delegate called when character starts to sprint
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWantsToSprint);

//Delegate called when character stops sprinting
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWantsToNotSprint);

//Delegate called when character starts taking damage
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamageSignature, float, Damage);

UCLASS()
class CHARACTERLOCOMOTION_API ACustomCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomCharacter(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void AddStartUpGameplayAbilities();	

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	TArray<TSubclassOf<UBaseGameplayAbility>> GameplayAbilities;

	UPROPERTY()
	bool bAbilitiesInitialized = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame by Custom Movement Component  to see if character wants to sprint.
	bool GetWantsToSprint();

	// Called every frame by Custom Movement Component to multiply default walk speed.
	float GetSprintSpeedModifier() const;

	//Public accessor to set the sprint speed modifier.  
	//Use case would be for stamina components to change the sprint speed modifer to 1 if stamina is 0.
	UFUNCTION()
	void SetSprintSpeedModifer(float NewSprintModifer);

	//Called from player input bound to sprint action.  Will toggle boolean 'bWantsToSprint' which is then processed by Custom Movement Component. 
	//Will perform RPC if not authority.  
	//This function is public so attributes can effect as well.
	//Use case would be for stamina component to force character to stop sprinting if not enough stamina.  
	UFUNCTION()
	void SetWantsToSprint();

	//Called every frame by Custom Movement Component to calculate character speed. 
	float GetDefaultWalkSpeed() const;

	//Delegate called when character starts to sprint
	UPROPERTY(BlueprintAssignable)
	FWantsToSprint WantsToSprint_OnSprintStart;

	//Delegate called when character stops sprinting
	UPROPERTY(BlueprintAssignable)
	FWantsToNotSprint WantsToNotSprint_OnSptrintStop;

	//Delegate called when character starts taking damage
	UPROPERTY(BlueprintAssignable)
	FOnTakeDamageSignature Damage_DamageTaken;

protected:

	// ==== Components ===== =====


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
		UBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UBaseAttributeSet* Attributes;

	// ==== Movement Functions ===== =====

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnRight(float Value);

	void LookUp(float Value);

	void Jump();

	void ToggleCrouch();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWantsToSprint();
	bool Server_SetWantsToSprint_Validate();
	void Server_SetWantsToSprint_Implementation();

	// ==== Movement Variables ===== =====

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player Movement")
		bool bWantsToSprint;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player Movement")
		bool bIsSprinting;

	UPROPERTY(EditDefaultsOnly, Category = "Player Movement")
		float DefaultWalkSpeed = 175;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Player Movement")
		float SprintSpeedMultiplier = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Player Movement")
		float DefaultCrouchSpeed = 150;

	// ==== Damage ====

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:

	void SetupAbilitiesInput();

};

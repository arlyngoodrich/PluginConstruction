// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWantsToSprint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWantsToNotSprint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamageSignature, float, Damage);

UCLASS()
class CHARACTERLOCOMOTION_API ACustomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomCharacter(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called by Movement component to see if character wants to sprint
	bool GetWantsToSprint();

	// Called by Movement component to multiply default walk speed
	float GetSprintSpeedModifier() const;

	UFUNCTION()
	void SetSprintSpeedModifer(float NewSprintModifer);

	UFUNCTION()
	void SetWantsToSprint();

	float GetDefaultWalkSpeed() const;

	UPROPERTY(BlueprintAssignable)
	FWantsToSprint WantsToSprint_OnSprintStart;

	UPROPERTY(BlueprintAssignable)
	FWantsToNotSprint WantsToNotSprint_OnSptrintStop;

	UPROPERTY(BlueprintAssignable)
	FOnTakeDamageSignature Damage_DamageTaken;

protected:

	// ==== Components ===== =====


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Components")
		UCameraComponent* CameraComp;


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

};

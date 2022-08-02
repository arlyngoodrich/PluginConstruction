// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "AbilitySystem/BaseHealthAttributeSet.h"
#include "GameFramework/Character.h"
#include "UniversalBaseCharacter.generated.h"

class UBaseGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UBaseAbilitySystemComponent;
class UBaseAttributeSet;

UCLASS()
class UNIVERSALCOREASSETS_API AUniversalBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUniversalBaseCharacter(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void AddStartUpGameplayAbilities();	

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	TArray<TSubclassOf<UBaseGameplayAbility>> GameplayAbilities;

	UPROPERTY()
	bool bAbilitiesInitialized = false;
	

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UBaseAttributeSet* Attributes;

	/**
 * @brief Called when the character takes damage, which may have also killed the character
 * @param DamageAmount Amount of damage that was done, not clamped based on current health 
 * @param HitInfo The hit info that generated this damage
 * @param DamageTags The gameplay tags of the event that did the damage
 * @param InstigatingCharacter The character that initiated this damage
 * @param DamageCauser The actual actor that did the damage, might be a weapon or projectile 
 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamage(float DamageAmount,const FHitResult& HitInfo,const struct FGameplayTagContainer& DamageTags,
		AUniversalBaseCharacter* InstigatingCharacter,AActor* DamageCauser);

	/**
	 * @brief Called when health is changed, either from healing or from being damaged
	 * For Damage this is called in addition to OnDamaged
	 * @param DeltaValue Change in health value, positive for heal and negative for damage.  If 0, the delta is unknown
	 * @param EventTags The gameplay tags of the vent that changed health
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChange(float DeltaValue,const struct FGameplayTagContainer& EventTags);

	virtual void HandleDamage(float DamageAmount,const FHitResult& HitInfo,const struct FGameplayTagContainer& DamageTags,
		AUniversalBaseCharacter* InstigatingCharacter,AActor* DamageCauser);

	virtual void HandleHealthChange(float DeltaValue,const struct FGameplayTagContainer& EventTags);

	friend UBaseHealthAttributeSet;

private:
	
	void SetupAbilitiesInput();

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseStaminaAttributeSet.generated.h"

// Uses macros from Attributes.h
#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class UNIVERSALCOREASSETS_API UBaseStaminaAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()

public:
	UBaseStaminaAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;


	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Stamina,Category="Attributes Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseStaminaAttributeSet,Stamina);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxStamina,Category="Attributes Stamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBaseStaminaAttributeSet,MaxStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_StaminaRegenRate,Category="Attributes Stamina")
	FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UBaseStaminaAttributeSet,StaminaRegenRate);

	UFUNCTION()
	virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldValue);


	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseStaminaAttributeSet.h"

#include "Net/UnrealNetwork.h"

UBaseStaminaAttributeSet::UBaseStaminaAttributeSet()
{
}

void UBaseStaminaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseStaminaAttributeSet,Stamina);
	DOREPLIFETIME(UBaseStaminaAttributeSet,MaxStamina);
}

void UBaseStaminaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UBaseStaminaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UBaseStaminaAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseStaminaAttributeSet,Stamina,OldValue);
}

void UBaseStaminaAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseStaminaAttributeSet,MaxStamina,OldValue);
}



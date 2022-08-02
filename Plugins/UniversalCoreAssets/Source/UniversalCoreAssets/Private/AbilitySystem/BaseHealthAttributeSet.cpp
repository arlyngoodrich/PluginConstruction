// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseHealthAttributeSet.h"
#include "Net/UnrealNetwork.h"

UBaseHealthAttributeSet::UBaseHealthAttributeSet()
{
	
}

void UBaseHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseHealthAttributeSet,Health);
}

void UBaseHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseHealthAttributeSet,Health,OldValue);
}

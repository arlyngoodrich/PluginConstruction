// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask_SuccessFailEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"


UAbilityTask_SuccessFailEvent::UAbilityTask_SuccessFailEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAbilityTask_SuccessFailEvent* UAbilityTask_SuccessFailEvent::WaitSuccessFailEvent(UGameplayAbility* OwningAbility,
                                                                                   FGameplayTag SuccessTag, FGameplayTag FailTag, AActor* OptionalExternalTarget, bool OnlyTriggerOnce,
                                                                                   bool OnlyMatchExact)
{
	UAbilityTask_SuccessFailEvent* MyObj = NewAbilityTask<UAbilityTask_SuccessFailEvent>(OwningAbility);
	MyObj->SuccessTag = SuccessTag;
	MyObj->FailTag = FailTag;
	MyObj->SetExternalTarget(OptionalExternalTarget);
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	MyObj->OnlyMatchExact = OnlyMatchExact;

	return MyObj;
}

void UAbilityTask_SuccessFailEvent::SetExternalTarget(AActor* Actor)
{
	if(Actor)
	{
		UseExternalTarget = true;
		OptionalExternalTarget = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
		
	}
}

UAbilitySystemComponent* UAbilityTask_SuccessFailEvent::GetTargetAsc() const
{
	if(UseExternalTarget)
	{
		return OptionalExternalTarget;
	}

	return AbilitySystemComponent;
}

void UAbilityTask_SuccessFailEvent::Activate()
{

	UAbilitySystemComponent* ASC = GetTargetAsc();
	if(ASC)
	{
		if(OnlyMatchExact)
		{
			SuccessHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(SuccessTag).AddUObject(
				this, &UAbilityTask_SuccessFailEvent::SuccessEventCallback);

			FailHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(FailTag).AddUObject(
				this, &UAbilityTask_SuccessFailEvent::FailEventCallback);
		}
		else
		{
			SuccessHandle = ASC->AddGameplayEventTagContainerDelegate(FGameplayTagContainer(SuccessTag),
			                                                          FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(
				                                                          this,
				                                                          &UAbilityTask_SuccessFailEvent::SuccessEventContainerCallback));
			
			FailHandle = ASC->AddGameplayEventTagContainerDelegate(FGameplayTagContainer(FailTag),
			                                                       FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(
				                                                       this,
				                                                       &UAbilityTask_SuccessFailEvent::FailEventContainerCallback));
		}
	}
	
	Super::Activate();
}

void UAbilityTask_SuccessFailEvent::SuccessEventCallback(const FGameplayEventData* Payload)
{
	SuccessEventContainerCallback(SuccessTag,Payload);
}

void UAbilityTask_SuccessFailEvent::SuccessEventContainerCallback(FGameplayTag MatchingTag,
	const FGameplayEventData* Payload)
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempPayload = *Payload;
		TempPayload.EventTag = MatchingTag;
		SuccessEventReceived.Broadcast(MoveTemp(TempPayload));
	}
	if(OnlyTriggerOnce)
	{
		EndTask();
	}
}

void UAbilityTask_SuccessFailEvent::FailEventCallback(const FGameplayEventData* Payload)
{
	FailEventContainerCallback(FailTag,Payload);
}

void UAbilityTask_SuccessFailEvent::FailEventContainerCallback(FGameplayTag MatchingTag,
	const FGameplayEventData* Payload)
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempPayload = *Payload;
		TempPayload.EventTag = MatchingTag;
		FailEventReceived.Broadcast(MoveTemp(TempPayload));
	}
	if(OnlyTriggerOnce)
	{
		EndTask();
	}
}

void UAbilityTask_SuccessFailEvent::OnDestroy(bool AbilityEnding)
{

	UAbilitySystemComponent* ASC = GetTargetAsc();
	
	if(ASC && SuccessHandle.IsValid())
	{
		if(OnlyMatchExact)
		{
			ASC->GenericGameplayEventCallbacks.FindOrAdd(SuccessTag).Remove(SuccessHandle);
		}
		else
		{
			ASC->RemoveGameplayEventTagContainerDelegate(FGameplayTagContainer(SuccessTag),SuccessHandle);
		}
	}

	if(ASC && FailHandle.IsValid())
	{
		if(OnlyMatchExact)
		{
			ASC->GenericGameplayEventCallbacks.FindOrAdd(FailTag).Remove(FailHandle);
		}
		else
		{
			ASC->RemoveGameplayEventTagContainerDelegate(FGameplayTagContainer(FailTag),FailHandle);
		}
	}
	
	Super::OnDestroy(AbilityEnding);
}

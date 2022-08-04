// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_SuccessFailEvent.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSuccessFailEventDelegate, FGameplayEventData, Payload);

/**
 * Custom ability task which uses a success or fail tag to decided between two executable branches 
 */
UCLASS()
class UNIVERSALCOREASSETS_API UAbilityTask_SuccessFailEvent : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FWaitSuccessFailEventDelegate	SuccessEventReceived;

	UPROPERTY(BlueprintAssignable)
	FWaitSuccessFailEventDelegate	FailEventReceived;

	/**
	 * Wait until the specified gameplay tag event is triggered. Allows for success or failure.
	 * By default this will look at the owner of this ability.
	 * OptionalExternalTarget can be set to make this look at another actor's tags for changes
	 * It will keep listening as long as OnlyTriggerOnce = false
	 * If OnlyMatchExact = false it will trigger for nested tags
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_SuccessFailEvent* WaitSuccessFailEvent(UGameplayAbility* OwningAbility, FGameplayTag SuccessTag,
	                                                           FGameplayTag FailTag,
	                                                           AActor* OptionalExternalTarget = nullptr,
	                                                           bool OnlyTriggerOnce = false,
	                                                           bool OnlyMatchExact = true);

	void SetExternalTarget(AActor* Actor);

	UAbilitySystemComponent* GetTargetAsc() const;

	virtual void Activate() override;

	virtual void SuccessEventCallback(const FGameplayEventData* Payload);
	virtual void SuccessEventContainerCallback(FGameplayTag MatchingTag, const FGameplayEventData* Payload);

	virtual void FailEventCallback(const FGameplayEventData* Payload);
	virtual void FailEventContainerCallback(FGameplayTag MatchingTag, const FGameplayEventData* Payload);

	void OnDestroy(bool AbilityEnding) override;

	FGameplayTag SuccessTag;

	FGameplayTag FailTag;

	UPROPERTY()
	UAbilitySystemComponent* OptionalExternalTarget;

	bool UseExternalTarget;	
	bool OnlyTriggerOnce;
	bool OnlyMatchExact;

	FDelegateHandle SuccessHandle;

	FDelegateHandle FailHandle;
	
};

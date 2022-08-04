// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbilityActor.h"

#include "UniversalData.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseGameplayAbility.h"


// Sets default values
ABaseAbilityActor::ABaseAbilityActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("Ability System Component"));
}

// Called when the game starts or when spawned
void ABaseAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	AddStartUpGameplayAbilities();
	
}

// Called every frame
void ABaseAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* ABaseAbilityActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}



void ABaseAbilityActor::AddStartUpGameplayAbilities()
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






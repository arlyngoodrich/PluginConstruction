// Fill out your copyright notice in the Description page of Project Settings.


#include "UniversalBaseCharacter.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/BaseGameplayAbility.h"

// Sets default values
AUniversalBaseCharacter::AUniversalBaseCharacter(const class FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//Instantiate the Attribute Sets and it automatically registers with the ASC 
	HealthAttributes = CreateDefaultSubobject<UBaseHealthAttributeSet>(TEXT("Health Attributes"));

}

// Called to bind functionality to input
void AUniversalBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupAbilitiesInput();
	

}

float AUniversalBaseCharacter::GetHealth()
{
	if(HealthAttributes)
	{
		return HealthAttributes->GetHealth();
	}
	return 1.f;
}

float AUniversalBaseCharacter::GetMaxHealth()
{
	if(HealthAttributes)
	{
		return HealthAttributes->GetMaxHealth();
	}
	return 1.f;
}

// Called when the game starts or when spawned
void AUniversalBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUniversalBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Server GAS init
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		AddStartUpGameplayAbilities();
	}
}

void AUniversalBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetupAbilitiesInput();
}

UAbilitySystemComponent* AUniversalBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AUniversalBaseCharacter::AddStartUpGameplayAbilities()
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

void AUniversalBaseCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, AUniversalBaseCharacter* InstigatingCharacter, AActor* DamageCauser)
{
	OnDamage(DamageAmount,HitInfo,DamageTags,InstigatingCharacter,DamageCauser);
}

void AUniversalBaseCharacter::HandleHealthChange(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if(bAbilitiesInitialized)
	{
		OnHealthChange(DeltaValue,EventTags);
	}
}

// Called every frame
void AUniversalBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AUniversalBaseCharacter::SetupAbilitiesInput()
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "BaseAbilityActor.generated.h"

class UBaseGameplayAbility;
class UGameplayEffect;
class UBaseAbilitySystemComponent;
UCLASS()
class UNIVERSALCOREASSETS_API ABaseAbilityActor : public AActor , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseAbilityActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void AddStartUpGameplayAbilities();	
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	TArray<TSubclassOf<UBaseGameplayAbility>> GameplayAbilities;

	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	bool bAbilitiesInitialized;

};

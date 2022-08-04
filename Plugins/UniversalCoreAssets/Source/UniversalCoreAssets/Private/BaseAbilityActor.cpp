// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbilityActor.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"


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




// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingComponent.h"
#include "ItemBase.h"
#include "ItemSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCraftingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCraftingComponent,EligibleCraftingRecipes);
}

// Called when the game starts
void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


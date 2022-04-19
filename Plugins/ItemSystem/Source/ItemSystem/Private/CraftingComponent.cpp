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

void UCraftingComponent::Debug_AddEligibleRecipe(const FCraftingRecipe NewRecipe)
{
	if(CanCraftRecipe(NewRecipe))
	{
		EligibleCraftingRecipes.Add(NewRecipe);
	}
}

int32 UCraftingComponent::GetNumValidRecipes() const {return EligibleCraftingRecipes.Num();}


void UCraftingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCraftingComponent,EligibleCraftingRecipes);
}


void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeRecipes();
	
}

void UCraftingComponent::OnRegister()
{
	
	InitializeRecipes();
	
}

void UCraftingComponent::InitializeRecipes()
{

	//Only execute on authority
	if(GetOwnerRole() != ROLE_Authority){return;}

	//Make sure crafting data table pointer is valid
	if(CraftingRecipeTable == nullptr)
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s crafting component failed to initialize recipes. Not DT is set."))
		return;
	}

	//Iterate through all rows
	const TArray<FName> CraftingRows = CraftingRecipeTable->GetRowNames();
	
	for (int i = 0; i < CraftingRows.Num(); ++i)
	{
		const FCraftingRecipe TargetRecipe = *CraftingRecipeTable->FindRow<FCraftingRecipe>(
			CraftingRows[i], "Crafting Recipe Initialization", true);

		if(CanCraftRecipe(TargetRecipe))
		{
			EligibleCraftingRecipes.Add(TargetRecipe);
		}
	}
	
	
}

bool UCraftingComponent::CanCraftRecipe(const FCraftingRecipe RecipeToCheck) const
{
	const TArray<TSubclassOf<UCraftingComponent>> EligibleComponents = RecipeToCheck.EligibleCraftingComponentTypes;

	for (int i = 0; i < EligibleComponents.Num(); ++i)
	{
		if(this->IsA(EligibleComponents[i]))
		{
			return true;
		}
	}
	
	 return false;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingComponent.h"

#include "InventoryComponent.h"
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
	if(IsComponentEligibleToCraftRecipe(NewRecipe))
	{
		EligibleCraftingRecipes.Add(NewRecipe);
		
		UE_LOG(LogItemSystem,Log,TEXT("Added debug recipe %s to %s crafting component"),
			*NewRecipe.RecipeName.ToString(),*GetOwner()->GetName())
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

	Super::OnRegister();
	
	InitializeRecipes();
	
}

void UCraftingComponent::SpawnExcessItem_Implementation(const FItemData ItemData)
{
	UE_LOG(LogItemSystem,Log,TEXT("%s crafting compmonent called Spawn Excees Item for %s item"),
		*GetOwner()->GetName(), *ItemData.DisplayName.ToString())
}

void UCraftingComponent::InitializeRecipes()
{

	//Only execute on authority
	if(GetOwnerRole() != ROLE_Authority){return;}

	//Make sure crafting data table pointer is valid
	if(CraftingRecipeTable == nullptr)
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s crafting component failed to initialize recipes. Not DT is set."),
			*GetOwner()->GetName())
		return;
	}

	//Iterate through all rows
	const TArray<FName> CraftingRows = CraftingRecipeTable->GetRowNames();
	
	for (int i = 0; i < CraftingRows.Num(); ++i)
	{
		const FCraftingRecipe TargetRecipe = *CraftingRecipeTable->FindRow<FCraftingRecipe>(
			CraftingRows[i], "Crafting Recipe Initialization", true);

		if(IsComponentEligibleToCraftRecipe(TargetRecipe))
		{
			EligibleCraftingRecipes.Add(TargetRecipe);
		}
	}
}

bool UCraftingComponent::IsComponentEligibleToCraftRecipe(const FCraftingRecipe RecipeToCheck) const
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

bool UCraftingComponent::CraftRecipe(FCraftingRecipe Recipe)
{

	//Check to see if able to craft recipe.  OK to perform on client so we're not doing an RPC if we can't 
	if(CraftRecipeChecks(Recipe) == false)
	{
		return false;
	}

	//Ensure only crafting on the server
	if(GetOwnerRole()!=ROLE_Authority)
	{
		return false;
	}

	//Make sure there are inventories to use for crafting
	TArray<UInventoryComponent*> InventoryComponents;
	if(GetInventories(InventoryComponents) == false)
	{
		 return false;
	}

	//Consume inputs from inventories
	const TArray<FRecipeComponent> Inputs = Recipe.RecipeInputs;
	TArray<bool> ConsumeInputChecks;
	for (int i = 0; i < Inputs.Num(); ++i)
	{
		bool ConsumeInputCheck = ConsumeComponentInput(Inputs[i],InventoryComponents);
		ConsumeInputChecks.Add(ConsumeInputCheck);
	}

	if(ConsumeInputChecks.Contains(false))
	{
		return false;
	}
	
	const TArray<FRecipeComponent> Outputs = Recipe.RecipeOutputs;
	for (int i = 0; i < Outputs.Num(); ++i)
	{
		DeliverRecipeOutput(Outputs[i],InventoryComponents);
	}

	return true;
	
}

bool UCraftingComponent::CraftRecipeChecks(const FCraftingRecipe Recipe) const
{

	//Ensure component can craft recipe
	if(IsComponentEligibleToCraftRecipe(Recipe) == false)
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s attempted to craft %s recipe but is not eligible to"),
			*GetOwner()->GetName(),*Recipe.RecipeName.ToString())
		return false;
	}

	//Get Eligible Inventory Components
	TArray<UInventoryComponent*> InventoryComponents;
	if(GetInventories(InventoryComponents) == false)
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s attempted to craft %s recipe but there are no inventories"),
		*GetOwner()->GetName(),*Recipe.RecipeName.ToString())
		return false;
	}
	
	//Ensure Inventories have enough inputs
	TArray<bool> InputChecks;
	const TArray<FRecipeComponent> Inputs = Recipe.RecipeInputs;
	for (int i = 0; i < Inputs.Num(); ++i)
	{
		bool InputCheck = InputComponentCheck(Inputs[i],InventoryComponents);
		InputChecks.Add(InputCheck);
	}

	if(InputChecks.Contains(false))
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s attempted to craft %s recipe but there are not enough inputs"),
		*GetOwner()->GetName(),*Recipe.RecipeName.ToString())
		return false;
	}
	
	return true;
	
}

bool UCraftingComponent::InputComponentCheck(const FRecipeComponent Component,TArray<UInventoryComponent*> InventoryComponents)
{
	int32 Quantity = 0;
	
	for (int i = 0; i < InventoryComponents.Num(); ++i)
	{
		UInventoryComponent* TargetInventory = InventoryComponents[i];
		Quantity += TargetInventory->GetTotalCountOfItemSubClass(Component.ComponentClass);
		UE_LOG(LogItemSystem,Log,TEXT("Total Input = %d"),Quantity)
	}


	return Quantity >= Component.Quantity;
}

// ReSharper disable once CppMemberFunctionMayBeConst -- calls non const method on Inventory Comp
bool UCraftingComponent::ConsumeComponentInput(const FRecipeComponent RecipeComponent,
                                               TArray<UInventoryComponent*> InventoryComponents)
{

	//Make sure enough inputs
	if(InputComponentCheck(RecipeComponent,InventoryComponents) == false)
	{
		 return false;
	}
	
	int32 QtyToConsume = RecipeComponent.Quantity;
	int32 QtyRemainingToConsume;

	//Cycle through inventories consuming input 
	for (int i = 0; i < InventoryComponents.Num(); ++i)
	{
		UInventoryComponent* TargetInventoryComponent = InventoryComponents[i];
		TargetInventoryComponent->ReduceQuantityOfItemByClassSubType(RecipeComponent.ComponentClass, QtyToConsume,
		                                                             QtyRemainingToConsume);
		//Set new Qty to consume to Qty remaining to consume 
		QtyToConsume = QtyRemainingToConsume;
	
		//Fully consumed input
		if(QtyToConsume == 0)
		{
			UE_LOG(LogItemSystem,Log,TEXT("%s crafting component consumed %d of %s"),
				*GetOwner()->GetName(),RecipeComponent.Quantity,*RecipeComponent.ComponentClass->GetName())
			return true;
		}
	}

	UE_LOG(LogItemSystem,Error,TEXT("%s crafting component could not fully remove inputs for %s component"),
		*GetOwner()->GetName(),*RecipeComponent.ComponentClass->GetName())
	return false;
}

// ReSharper disable once CppMemberFunctionMayBeConst -- calls non const method on Inventory Comp
void UCraftingComponent::DeliverRecipeOutput(const FRecipeComponent RecipeOutput,
                                             TArray<UInventoryComponent*> InventoryComponents)
{
	const AItemBase* DefaultItem = RecipeOutput.ComponentClass.GetDefaultObject();

	//Create Item Data from Item Class Defaults
	FItemData ItemData = DefaultItem->GetItemData();
	ItemData.ItemGuid = FGuid::NewGuid();
	ItemData.InWorldClass = RecipeOutput.ComponentClass;
	ItemData.bIsValid = true;

	FItemData RemainingItemData = ItemData;
	
	for (int i = 0; i < InventoryComponents.Num(); ++i)
	{
		UInventoryComponent* TargetInventory = InventoryComponents[i];
		if(TargetInventory->AutoAddItem(ItemData,RemainingItemData))
		{
			UE_LOG(LogItemSystem,Log,TEXT("%s crafting component added %s to %s inventory"),
				*GetOwner()->GetName(),*ItemData.DisplayName.ToString(),*TargetInventory->GetName())
			break;
		}
		else
		{
			ItemData = RemainingItemData;
		}
	}

	if(RemainingItemData.ItemQuantity > 0)
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s could not place %d of %s item in inventory"),
			*GetOwner()->GetName(),RemainingItemData.ItemQuantity,*RemainingItemData.DisplayName.ToString())

		SpawnExcessItem(RemainingItemData);
	}
	
}

bool UCraftingComponent::GetInventories(TArray<UInventoryComponent*>& OutInventoryComponents) const
{
	const AActor* Owner = GetOwner();
	Owner->GetComponents<UInventoryComponent>(OutInventoryComponents);
	
	return (OutInventoryComponents.Num() > 0 );
}




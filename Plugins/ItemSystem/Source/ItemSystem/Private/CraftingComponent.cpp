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

void UCraftingComponent::Debug_AddEligibleRecipe(FCraftingRecipe NewRecipe)
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
	DOREPLIFETIME(UCraftingComponent,ActiveRecipe);
	DOREPLIFETIME(UCraftingComponent,bIsActivelyCrafting);
	DOREPLIFETIME(UCraftingComponent,CraftingQueue);
	
	//DOREPLIFETIME_CONDITION_NOTIFY(UCraftingComponent,ActiveRecipe,COND_None,REPNOTIFY_Always);
}


void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeRecipes();
	UpdateInventories();
	
	
}

void UCraftingComponent::OnRegister()
{

	Super::OnRegister();
	InitializeRecipes();
	UpdateInventories();
	
}

void UCraftingComponent::SpawnExcessItem(const FItemData ItemData)
{
	if(InventoryComponents[0]->DropItem(ItemData) == false)
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s crafting compmonent could not spawn excesss for %s item"),
			*GetOwner()->GetName(), *ItemData.DisplayName.ToString())
	}
}

void UCraftingComponent::InitializeRecipes()
{
	
	//Only execute on authority
	if(GetOwnerRole() != ROLE_Authority){return;}

	if(bRecipesInitialized){return;}
	
	//Make sure crafting data table pointer is valid
	if(CraftingRecipeTable == nullptr)
	{
		UE_LOG(LogItemSystem,Log,TEXT("%s crafting component failed to initialize recipes. Not DT is set."),
			*GetOwner()->GetName())
		return;
	}

	//Iterate through all rows
	const TArray<FName> CraftingRows = CraftingRecipeTable->GetRowNames();

	EligibleCraftingRecipes.Empty();
	
	for (int i = 0; i < CraftingRows.Num(); ++i)
	{
		FCraftingRecipe TargetRecipe = *CraftingRecipeTable->FindRow<FCraftingRecipe>(
			CraftingRows[i], "Crafting Recipe Initialization", true);

		//Make sure can craft recipe and not already in array
		if(IsComponentEligibleToCraftRecipe(TargetRecipe) && !EligibleCraftingRecipes.Contains(TargetRecipe))
		{
			
			EligibleCraftingRecipes.Add(TargetRecipe);
		}
	}

	bRecipesInitialized = true;
}


// ReSharper disable once CppMemberFunctionMayBeConst
void UCraftingComponent::OnInventoryUpdate()
{
	CraftingUIUpdate.Broadcast();
}

void UCraftingComponent::StartCraftingTimer(const FCraftingRecipe Recipe)
{
	ActiveRecipe = Recipe;
	bIsActivelyCrafting = true;

	if(GetOwnerRole() == ROLE_Authority)
	{
		OnRep_ActiveRecipeSet();
	}

	if(Recipe.CraftTime > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CraftingTimerHandle,this,&UCraftingComponent::FinalizeCrafting,Recipe.CraftTime,false);
		Client_CraftingStarted(Recipe.CraftTime);
		OnCraftingStarted.Broadcast(Recipe.CraftTime);
		
	}
	else
	{
		FinalizeCrafting();
	}
	
}


void UCraftingComponent::FinalizeCrafting()
{
	DeliverRecipeOutput(ActiveRecipe.RecipeOutputs,InventoryComponents);
	GetWorld()->GetTimerManager().ClearTimer(CraftingTimerHandle);
	bIsActivelyCrafting = false;
	ActiveRecipe.Invalidate();

	Client_CraftingFinished();
	CraftFromQueue();
}

void UCraftingComponent::AddRecipeToQueue(const FCraftingRecipe Recipe, const int32 RecipeQty)
{

	if(GetOwnerRole()!=ROLE_Authority){return;}
	
	//Find the current max slot
	int32 NewSlotPosition;
	if(CraftingQueue.Num()>0)
	{
		TArray<int32> QueueSlots;
		for (int i = 0; i < CraftingQueue.Num(); ++i)
		{
			QueueSlots.Add(CraftingQueue[i].SlotPosition);
		}

		NewSlotPosition = FMath::Max(QueueSlots)+1;
	}
	else
	{
		NewSlotPosition = 0;
	}

	const FCraftingQueueSlot NewSlot = FCraftingQueueSlot(Recipe,NewSlotPosition,RecipeQty);

	//ON REP?
	CraftingQueue.Add(NewSlot);

	UE_LOG(LogItemSystem,Log,TEXT("%s added %s to queue at position %d"),
		*GetOwner()->GetName(),*Recipe.RecipeName.ToString(),NewSlotPosition)
}

void UCraftingComponent::CraftFromQueue()
{
	if(GetOwnerRole()!=ROLE_Authority){return;}
	
	if(CraftingQueue.Num()==0){return;}

	//Find the index for slot 0
	int32 IndexSlot0 = 0;
	bool bFoundSlot0 = false;
	for (int i = 0; i < CraftingQueue.Num(); ++i)
	{
		if(CraftingQueue[i].SlotPosition == 0)
		{
			IndexSlot0 = i;
			bFoundSlot0 = true;
			break;
		}
	}

	if(bFoundSlot0 == false)
	{
		UE_LOG(LogItemSystem,Error,TEXT("%s attempted to craft from queue but could not find queue slot 0"),
			*GetOwner()->GetName())
		return;
	}
	
	CraftRecipe(CraftingQueue[IndexSlot0].Recipe);
	//OnRep
	CraftingQueue.RemoveAt(IndexSlot0);
	
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

bool UCraftingComponent::CraftRecipe(const FCraftingRecipe Recipe)
{
	UpdateInventories();

	//Check to see if able to craft recipe.  OK to perform on client so we're not doing an RPC if we can't 
	if(CraftRecipeChecks(Recipe) == false)
	{
		return false;
	}

	//Ensure only crafting on the server
	if(GetOwnerRole()!=ROLE_Authority)
	{
		Server_RequestCraftRecipe(Recipe);
		return true;
	}

	//Make sure there are inventories to use for crafting
	if(UpdateInventories() == false)
	{
		 return false;
	}

	if(bIsActivelyCrafting)
	{
		AddRecipeToQueue(Recipe,1);
		return true;
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
	
	StartCraftingTimer(Recipe);
		
	return true;
	
}

TArray<FCraftingRecipe> UCraftingComponent::GetEligibleCraftingRecipes() {return EligibleCraftingRecipes;}

int32 UCraftingComponent::GetAvailableQtyOfItem(const FItemData ItemData) const
{
	int32 ItemQuantity = 0;

	for (int i = 0; i < InventoryComponents.Num(); ++i)
	{
		ItemQuantity += InventoryComponents[i]->GetTotalCountOfItemSubClass(ItemData.InWorldClass);
	}

	return ItemQuantity;
}

bool UCraftingComponent::CanRecipeBeCrafted(const FCraftingRecipe Recipe)
{
	UpdateInventories();
	return CraftRecipeChecks(Recipe);
}


bool UCraftingComponent::CraftRecipeChecks(const FCraftingRecipe Recipe) const
{

	//Ensure component can craft recipe
	if(IsComponentEligibleToCraftRecipe(Recipe) == false)
	{
		//UE_LOG(LogItemSystem,Log,TEXT("%s attempted to craft %s recipe but is not eligible to"),
		//	*GetOwner()->GetName(),*Recipe.RecipeName.ToString())
		return false;
	}

	//Get Eligible Inventory Components
	if(InventoryComponents.Num() == 0)
	{
		//UE_LOG(LogItemSystem,Log,TEXT("%s attempted to craft %s recipe but there are no inventories"),
		//*GetOwner()->GetName(),*Recipe.RecipeName.ToString())
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
		//UE_LOG(LogItemSystem,Log,TEXT("%s attempted to craft %s recipe but there are not enough inputs"),
		//*GetOwner()->GetName(),*Recipe.RecipeName.ToString())
		return false;
	}
	
	return true;
	
}

bool UCraftingComponent::InputComponentCheck(const FRecipeComponent Component,TArray<UInventoryComponent*> InventoryComponentsRef)
{
	int32 Quantity = 0;
	
	for (int i = 0; i < InventoryComponentsRef.Num(); ++i)
	{
		UInventoryComponent* TargetInventory = InventoryComponentsRef[i];
		Quantity += TargetInventory->GetTotalCountOfItemSubClass(Component.ComponentClass);
		UE_LOG(LogItemSystem,Verbose,TEXT("Total Input = %d"),Quantity)
	}
	
	return Quantity >= Component.Quantity;
}


// ReSharper disable once CppMemberFunctionMayBeConst -- calls non const method on Inventory Comp
bool UCraftingComponent::ConsumeComponentInput(const FRecipeComponent RecipeComponent,
                                               TArray<UInventoryComponent*> InventoryComponentsRef)
{

	//Make sure enough inputs
	if(InputComponentCheck(RecipeComponent,InventoryComponentsRef) == false)
	{
		 return false;
	}
	
	int32 QtyToConsume = RecipeComponent.Quantity;
	int32 QtyRemainingToConsume;

	//Cycle through inventories consuming input 
	for (int i = 0; i < InventoryComponentsRef.Num(); ++i)
	{
		UInventoryComponent* TargetInventoryComponent = InventoryComponentsRef[i];
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
                                             TArray<UInventoryComponent*> InventoryComponentsRef)
{

	//Create Item Data from Item Class Defaults
	FItemData ItemData = RecipeOutput.ComponentClass.GetDefaultObject()->GetItemData();
	ItemData.SetFromDefaultObject(RecipeOutput.ComponentClass);

	FItemData RemainingItemData = ItemData;
	
	for (int i = 0; i < InventoryComponentsRef.Num(); ++i)
	{
		UInventoryComponent* TargetInventory = InventoryComponentsRef[i];
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

bool UCraftingComponent::UpdateInventories()
{
	const AActor* Owner = GetOwner();
	TArray<UInventoryComponent*> NewInventoryComponents;
	Owner->GetComponents<UInventoryComponent>(NewInventoryComponents);

	//Updates bindings and sets New Inventory Components Ref
	UpdateInventoryBindings(NewInventoryComponents);
	
	return (NewInventoryComponents.Num() > 0 );
}

void UCraftingComponent::UpdateInventoryBindings(const TArray<UInventoryComponent*> NewInventoryComponents)
{
	
	//Remove old dynamic bindings	
	for (int i = 0; i < InventoryComponents.Num(); ++i)
	{
		if(UInventoryComponent* TargetInventory = InventoryComponents[i])
		{
			TargetInventory->OnInventoryUpdate.RemoveDynamic(this,&UCraftingComponent::OnInventoryUpdate);
		}
		
	}

	//I know.. empty isn't needed 
	InventoryComponents.Empty();
	InventoryComponents = NewInventoryComponents;

	//Add new bindings
	for (int i = 0; i < InventoryComponents.Num(); ++i)
	{
		if(UInventoryComponent* TargetInventory = InventoryComponents[i])
		{
			TargetInventory->OnInventoryUpdate.AddDynamic(this,&UCraftingComponent::OnInventoryUpdate);
		}
		
	}
	
}

bool UCraftingComponent::Server_RequestCraftRecipe_Validate(FCraftingRecipe Recipe)
{
	return true;
}

void UCraftingComponent::Server_RequestCraftRecipe_Implementation(FCraftingRecipe Recipe)
{
	CraftRecipe(Recipe);
}

void UCraftingComponent::Client_CraftingStarted_Implementation(const float CraftDuration)
{
	//Client proxy timer for when crafting should approximately finish
	GetWorld()->GetTimerManager().SetTimer(CraftingTimerHandle,CraftDuration,false);
	OnCraftingStarted.Broadcast(CraftDuration);
}

void UCraftingComponent::Client_CraftingFinished_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(CraftingTimerHandle);
}



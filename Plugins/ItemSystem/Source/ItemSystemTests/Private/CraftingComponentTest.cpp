
#include "CoreMinimal.h"
#include "ItemSystem/Public/CraftingComponent.h"
#include "ItemSystem/Public/InventoryComponent.h"
#include "ItemSystem/Public/PlayerCraftingComponent.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"



IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddRecipeTest, "CraftingComponent.AddRecipies",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddRecipeTest::RunTest(const FString& Parameters)
{
	//Create World
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//Create Actor
	AActor* Actor = World->SpawnActor<AActor>();
	TestTrue(TEXT("Test Actor is not valid"),IsValid(Actor));

	//Create and Add Component to Actor
	const FName CompName("CraftingComp");
	UCraftingComponent* CraftingComponent = NewObject<UCraftingComponent>(Actor,CompName);
	TestTrue(TEXT("Crafting Comp is not valid"),IsValid(CraftingComponent));

	CraftingComponent->RegisterComponent();

	//Create Craftable Recipe
	const FRecipeComponent RecipeComponent = FRecipeComponent(AItemBase::StaticClass(),1);
	TArray<FRecipeComponent> Inputs;
	const FRecipeComponent Outputs = RecipeComponent;
	TArray<TSubclassOf<UCraftingComponent>> CraftingComponents;
	
	Inputs.Add(RecipeComponent);
	CraftingComponents.Add(UCraftingComponent::StaticClass());
	const FCraftingRecipe Recipe1 = FCraftingRecipe("TestRecipe1",Inputs,Outputs,CraftingComponents);

	CraftingComponent->Debug_AddEligibleRecipe(Recipe1);
	
	//Create Non-Craftable Recipe
	TArray<TSubclassOf<UCraftingComponent>> CraftingComponents1;
	CraftingComponents1.Add(UPlayerCraftingComponent::StaticClass());
	const FCraftingRecipe Recipe2 = FCraftingRecipe("TestRecipe1",Inputs,Outputs,CraftingComponents1);

	CraftingComponent->Debug_AddEligibleRecipe(Recipe2);

	//Test to see if there is 1 eligible recipe
	TestEqual(TEXT("Eligible Recipe Test"),CraftingComponent->GetNumValidRecipes(),1);
	
	// Make the test pass by returning true, or fail by returning false.
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCraftRecipe, "CraftingComponent.CraftRecipe",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCraftRecipe::RunTest(const FString& Parameters)
{
	//Create World
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//Create Actor
	AActor* Actor = World->SpawnActor<AActor>();
	TestTrue(TEXT("Test Actor is not valid"),IsValid(Actor));

	//Create and Add Component to Actor
	const FName InvCompName("InventoryComp");
	UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>(Actor,InvCompName);
	TestTrue(TEXT("Invetory Comp is not valid"),IsValid(InventoryComponent));

	InventoryComponent->RegisterComponent();
	TestTrue(TEXT("Inventory failed to initialize"),InventoryComponent->GetSlotCount()>0);

	//Create and Add Component to Actor
	const FName CraftCompName("CraftingComp");
	UCraftingComponent* CraftingComponent = NewObject<UCraftingComponent>(Actor,CraftCompName);
	TestTrue(TEXT("Crafting Comp is not valid"),IsValid(CraftingComponent));

	CraftingComponent->RegisterComponent();

	//Create Craftable Recipe
	const FRecipeComponent RecipeInput = FRecipeComponent(AItemBase::StaticClass(),2);
	const FRecipeComponent RecipeOutput = FRecipeComponent(AItemBase::StaticClass(),1);
	TArray<FRecipeComponent> Inputs;
	const FRecipeComponent Outputs = RecipeOutput;
	TArray<TSubclassOf<UCraftingComponent>> CraftingComponents;
	
	Inputs.Add(RecipeInput);
	CraftingComponents.Add(UCraftingComponent::StaticClass());
	const FCraftingRecipe Recipe = FCraftingRecipe("TestRecipe1",Inputs,Outputs,CraftingComponents);

	//Add Recipe to Eligible Recipe List
	CraftingComponent->Debug_AddEligibleRecipe(Recipe);

	//Create Test Items for Inputs
	const FName ItemName = "TestItem";
	const FItemData InputItemData = FItemData::NewItem(ItemName,AItemBase::StaticClass());

	InventoryComponent->AutoAddItem(InputItemData);
	InventoryComponent->AutoAddItem(InputItemData);

	//Craft Item
	const bool bRecipeSuccess = CraftingComponent->CraftRecipe(Recipe);

	TestTrue(TEXT("Recipe Crafted Test"),bRecipeSuccess);
	
	// Make the test pass by returning true, or fail by returning false.
	return true;
}



#include "CoreMinimal.h"
#include "CraftingComponent.h"
#include "PlayerCraftingComponent.h"
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
	TArray<FRecipeComponent> Outputs;
	TArray<TSubclassOf<UCraftingComponent>> CraftingComponents;
	
	Inputs.Add(RecipeComponent);
	Outputs.Add(RecipeComponent);
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

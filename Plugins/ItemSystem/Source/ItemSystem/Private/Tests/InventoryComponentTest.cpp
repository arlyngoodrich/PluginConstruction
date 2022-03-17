#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"
#include "InventoryComponent.h"
#include "ItemSystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddItemTest, "Inventory.AddItem",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddItemTest::RunTest(const FString& Parameters)
{

	//Create World
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//Create Actor
	AActor* Actor = World->SpawnActor<AActor>();
	TestTrue(TEXT("Test Actor is not valid"),IsValid(Actor));

	//Create and Add Component to Actor
	const FName CompName("InventoryComp");
	UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>(Actor,CompName);
	TestTrue(TEXT("Invetory Comp is not valid"),IsValid(InventoryComponent));

	InventoryComponent->RegisterComponent();
	TestTrue(TEXT("Inventory failed to initialize"),InventoryComponent->SlotNum()>0);

	
	
	
	
	return true;
	
}




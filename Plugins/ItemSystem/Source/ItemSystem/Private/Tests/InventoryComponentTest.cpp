#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"
#include "InventoryComponent.h"
#include "ItemBase.h"
#include "ItemSystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSimpleAddItemTest,"Inventory.SimpleAddItem",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSimpleAddItemTest::RunTest(const FString& Parameters)
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

	//Create item to add to inventory
	const FItemData NewItem = FItemData::NewItem
		(
			"TestItem",
			AItemBase::StaticClass(),
			FInventory2D(1,1),
			1,
			false,
			1.f
		);

	const bool bItemAdded = InventoryComponent->AddItemToPosition(NewItem,FInventory2D(1,1));
	TestTrue(TEXT("Item was not added to inventory"),bItemAdded);
	
	UE_LOG(LogItemSystem,Log,TEXT("%s result: Item was successfully added"),*GetBeautifiedTestName())
	return true;
}




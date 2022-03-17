#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"
#include "InventoryComponent.h"
#include "ItemBase.h"
#include "ItemSystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryAddItemTest,"Inventory.AddItem",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInventoryAddItemTest::RunTest(const FString& Parameters)
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
	TestTrue(TEXT("Inventory failed to initialize"),InventoryComponent->GetSlotCount()>0);

	//Test to make sure invalid item cannot be added
	const bool bInvalidItemAdded = InventoryComponent->AddItemToPosition(FItemData(),FInventory2D(1,1));
	TestTrue(TEXT("Invalid items was added to inventory"),bInvalidItemAdded==false);
	
	//Create a valid item to add to inventory
	const FInventory2D ItemSize = FInventory2D(1,1);
	const FItemData NewItem = FItemData::NewItem
		(
			"TestItem",
			AItemBase::StaticClass(),
			ItemSize,
			1,
			false,
			1.f
		);

	//Add valid item to inventory
	const FInventory2D ItemPosition = FInventory2D(1,1);
	const bool bValidItemAdded = InventoryComponent->AddItemToPosition(NewItem,ItemPosition);

	//Double check to make sure it is in the inventory
	FInventory2D ActualItemPosition;
	const bool bDoubleCheckInventory = InventoryComponent->IsItemInInventory(NewItem,ActualItemPosition);

	//Make sure item has been added and is in correct position
	TestTrue(TEXT("Item was not added to inventory"),bValidItemAdded&&bDoubleCheckInventory);
	TestTrue(TEXT("Item is not in expected position"), ActualItemPosition == ItemPosition);
	
	//Make sure weight was added to inventory
	TestEqual(TEXT("Weight was not added to inventory"),
		InventoryComponent->GetInventoryWeight(),
		NewItem.GetStackWeight());
	
	UE_LOG(LogItemSystem,Log,TEXT("%s result: Item was successfully added"),*GetBeautifiedTestName())	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryAutoAddItemTest,"Inventory.AutoAddItem",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInventoryAutoAddItemTest::RunTest(const FString& Parameters)
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
	TestTrue(TEXT("Inventory failed to initialize"),InventoryComponent->GetSlotCount()>0);

	//Create a valid item to add to inventory
	const FItemData NewItem = FItemData::NewItem
		(
			"TestItem",
			AItemBase::StaticClass(),
			FInventory2D(1,1),
			1,
			false,
			1.f
		);

	//Make sure item was added
	const bool bAutoAddSuccess = InventoryComponent->AutoAddItem(NewItem);
	const bool bWasItemAdded = InventoryComponent->IsItemInInventory(NewItem);
	TestTrue(TEXT("Item was not added to inventory"),bAutoAddSuccess&&bWasItemAdded);
	
	UE_LOG(LogItemSystem,Log,TEXT("%s result: Item was successfully added"),*GetBeautifiedTestName())	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryMaxWeightTest,"Inventory.MaxWeightTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInventoryMaxWeightTest::RunTest(const FString& Parameters)
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
	TestTrue(TEXT("Inventory failed to initialize"),InventoryComponent->GetSlotCount()>0);
		
	//Create a valid item to add to inventory
	const FItemData NewItem = FItemData::NewItem
		(
			"TestItem",
			AItemBase::StaticClass(),
			FInventory2D(1,1),
			2,
			false,
			InventoryComponent->GetInventoryMaxWeight()
		);

	//Make sure item was added
	const bool bAutoAddSuccess = InventoryComponent->AutoAddItem(NewItem);
	const bool bWasItemAdded = InventoryComponent->IsItemInInventory(NewItem);
	TestFalse(TEXT("Item wieght exceeds max but was added to inventory"),bAutoAddSuccess&&bWasItemAdded);
	
	UE_LOG(LogItemSystem, Log, TEXT("%s result: Item weight exceed max and was successfully NOT added"),
	       *GetBeautifiedTestName())
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventorySlotTest,"Inventory.SlotTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInventorySlotTest::RunTest(const FString& Parameters)
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
	TestTrue(TEXT("Inventory failed to initialize"),InventoryComponent->GetSlotCount()>0);
		
	//Create a valid item to add to inventory
	const FItemData NewItem = FItemData::NewItem
		(
			"TestItem",
			AItemBase::StaticClass(),
			FInventory2D(1,1),
			1,
			false,
			1.f
		);

	int32 ItemsNotAdded = 0;
	constexpr int32 NumberOverMax = 2;
	
	for (int i = 0; i < InventoryComponent->GetSlotCount()+NumberOverMax; ++i)
	{
		if(InventoryComponent->AutoAddItem(NewItem)==false)
		{
			ItemsNotAdded+=1;
		}
	}

	//Make sure the number of items we attempted to add over the max was not added
	TestEqual(TEXT("Items not added do not equal items added over max"),ItemsNotAdded,NumberOverMax);

	//Make sure we still added them maximum number of items
	TestEqual(TEXT("The number of items in the inventory is not equal to the max number of slots"),
		InventoryComponent->GetItemCount(),InventoryComponent->GetSlotCount());
	
	UE_LOG(LogItemSystem, Log, TEXT("%s result: The number of items added matches the max amount of slots"),
		   *GetBeautifiedTestName())
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWeightSummationTest,"Inventory.WeightSummationTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWeightSummationTest::RunTest(const FString& Parameters)
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
	TestTrue(TEXT("Inventory failed to initialize"),InventoryComponent->GetSlotCount()>0);
		
	//Create a valid item to add to inventory
	const FItemData NewItem = FItemData::NewItem
		(
			"TestItem",
			AItemBase::StaticClass(),
			FInventory2D(1,1),
			1,
			false,
			2.f
		);

	int32 NumItemsAdded = 0;
	for (int i = 0; i < InventoryComponent->GetSlotCount(); ++i)
	{
		if(InventoryComponent->AutoAddItem(NewItem))
		{
			NumItemsAdded +=1; 
		}
	}

	TestTrue(TEXT("Items were not added. Cannot check weight"),NumItemsAdded > 0);

	const float ActualInventoryWeight = InventoryComponent->GetInventoryWeight();
	const float ExpectedInventoryWeight = NewItem.PerItemWeight * NumItemsAdded;

	TestEqual(TEXT("Actual and expected weights are different"),ActualInventoryWeight,ExpectedInventoryWeight);
	
	UE_LOG(LogItemSystem, Log, TEXT("%s result: Multiple items were added and the inventory weight is correct"),
		   *GetBeautifiedTestName())
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryPartiallyRemoveItem,"Inventory.PartiallyRemoveItem",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInventoryPartiallyRemoveItem::RunTest(const FString& Parameters)
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
	TestTrue(TEXT("Inventory failed to initialize"),InventoryComponent->GetSlotCount()>0);
	
	//Create a valid item to add to inventory
	constexpr int32 ItemQuantity = 3;
	const FItemData NewItem = FItemData::NewItem
		(
			"TestItem",
			AItemBase::StaticClass(),
			FInventory2D(1,1),
			ItemQuantity,
			false,
			1.f
		);

	//Make sure item was added
	const bool bAutoAddSuccess = InventoryComponent->AutoAddItem(NewItem);
	TestTrue(TEXT("Item was not added to inventory"),bAutoAddSuccess);

	int32 QuantityRemaining;
	const bool bWasClassFoundToRemove = InventoryComponent->ReduceQuantityOfItemByStaticClass(
		AItemBase::StaticClass(), ItemQuantity - 1, QuantityRemaining);

	TestTrue(TEXT("Class was not found to be removed"),bWasClassFoundToRemove);

	//Make sure there is still one item in the inventory
	const int32 ItemsInInventory = InventoryComponent->GetItemCount();
	TestTrue(TEXT("There is not 1 item in inventory"),ItemsInInventory == 1);

	//Make sure weight was reduced
	const float ExpectedWeight = NewItem.PerItemWeight;
	const float InventoryWeight = InventoryComponent->GetInventoryWeight();
	TestEqual(TEXT("Inventory weight is different than expected"),InventoryWeight,ExpectedWeight);

	//UE_LOG(LogItemSystem,Log,TEXT("%s result: Item was successfully partially removed"),*GetBeautifiedTestName())	
	return true;
}
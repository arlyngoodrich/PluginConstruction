#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"
#include "InventoryComponent.h"
#include "ItemBase.h"

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
			1,
			false,
			1.f
		);

	//Make sure item was added
	const bool bAutoAddSuccess = InventoryComponent->AutoAddItem(NewItem);
	const bool bWasItemAdded = InventoryComponent->IsItemInInventory(NewItem);
	TestTrue(TEXT("Item was not added to inventory"),bAutoAddSuccess&&bWasItemAdded);
	
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
			2,
			false,
			InventoryComponent->GetInventoryMaxWeight()
		);

	//Make sure item was added
	const bool bAutoAddSuccess = InventoryComponent->AutoAddItem(NewItem);
	const bool bWasItemAdded = InventoryComponent->IsItemInInventory(NewItem);
	TestFalse(TEXT("Item wieght exceeds max but was added to inventory"),bAutoAddSuccess&&bWasItemAdded);
	
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
	TestTrue(TEXT("Test Actor is not valid"), IsValid(Actor));

	//Create and Add Component to Actor
	const FName CompName("InventoryComp");
	UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>(Actor, CompName);
	TestTrue(TEXT("Invetory Comp is not valid"), IsValid(InventoryComponent));

	InventoryComponent->RegisterComponent();
	TestTrue(TEXT("Inventory failed to initialize"), InventoryComponent->GetSlotCount() > 0);

	//Create a valid item to add to inventory
	constexpr int32 ItemQuantity = 3;
	const FItemData NewItem = FItemData::NewItem
	(
		"TestItem",
		AItemBase::StaticClass(),
		FInventory2D(1, 1),
		ItemQuantity,
		ItemQuantity,
		false,
		1.f
	);

	//Make sure item was added
	const bool bAutoAddSuccess = InventoryComponent->AutoAddItem(NewItem);
	TestTrue(TEXT("Item was not added to inventory"), bAutoAddSuccess);

	int32 QuantityRemaining;
	const bool bWasClassFoundToRemove = InventoryComponent->ReduceQuantityOfItemByStaticClass(
		AItemBase::StaticClass(), ItemQuantity - 1, QuantityRemaining);

	TestTrue(TEXT("Class was not found to be removed"), bWasClassFoundToRemove);

	//Make sure there is still one item in the inventory
	const int32 ItemsInInventory = InventoryComponent->GetItemCount();
	TestTrue(TEXT("There is not 1 item in inventory"), ItemsInInventory == 1);

	//Make sure weight was reduced
	const float ExpectedWeight = NewItem.PerItemWeight;
	const float InventoryWeight = InventoryComponent->GetInventoryWeight();
	TestEqual(TEXT("Inventory weight is different than expected"), InventoryWeight, ExpectedWeight);
	
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryFullyRemoveItem,"Inventory.FullyRemoveItem",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInventoryFullyRemoveItem::RunTest(const FString& Parameters)
{
	//Create World
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//Create Actor
	AActor* Actor = World->SpawnActor<AActor>();
	TestTrue(TEXT("Test Actor is not valid"), IsValid(Actor));

	//Create and Add Component to Actor
	const FName CompName("InventoryComp");
	UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>(Actor, CompName);
	TestTrue(TEXT("Invetory Comp is not valid"), IsValid(InventoryComponent));

	InventoryComponent->RegisterComponent();
	TestTrue(TEXT("Inventory failed to initialize"), InventoryComponent->GetSlotCount() > 0);

	//Create a valid item to add to inventory
	constexpr int32 ItemQuantity = 3;
	const FItemData NewItem = FItemData::NewItem
	(
		"TestItem",
		AItemBase::StaticClass(),
		FInventory2D(1, 1),
		ItemQuantity,
		ItemQuantity,
		false,
		1.f
	);

	//Make sure item was added
	const FInventory2D Position = FInventory2D(1,1);
	const bool bAutoAddSuccess = InventoryComponent->AddItemToPosition(NewItem,Position);
	TestTrue(TEXT("Item was not added to inventory"), bAutoAddSuccess);

	//Make sure Item can be removed
	int32 QuantityRemaining;
	const bool bWasInventoryItemFound = InventoryComponent->ReduceQuantityOfInventoryItem(
		FInventoryItemData(Position,NewItem), ItemQuantity, QuantityRemaining);
	TestTrue(TEXT("Inventory Item was not found to be removed"), bWasInventoryItemFound);

	//Make sure there is still are no items in inventory
	const int32 ItemsInInventory = InventoryComponent->GetItemCount();
	TestTrue(TEXT("There are still items in inventory"), ItemsInInventory == 0);

	//Make sure weight was reduced
	constexpr float ExpectedWeight = 0.f;
	const float InventoryWeight = InventoryComponent->GetInventoryWeight();
	TestEqual(TEXT("Inventory weight is different than expected"), InventoryWeight, ExpectedWeight);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStackItemTest,"Inventory.StackItem",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FStackItemTest::RunTest(const FString& Parameters)
{
	//Create World
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//Create Actor
	AActor* Actor = World->SpawnActor<AActor>();
	TestTrue(TEXT("Test Actor is not valid"), IsValid(Actor));

	//Create and Add Component to Actor
	const FName CompName("InventoryComp");
	UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>(Actor, CompName);
	TestTrue(TEXT("Invetory Comp is not valid"), IsValid(InventoryComponent));

	InventoryComponent->RegisterComponent();
	TestTrue(TEXT("Inventory failed to initialize"), InventoryComponent->GetSlotCount() > 0);

	//Create a valid item to add to inventory
	constexpr int32 ItemQuantity = 2;
	constexpr int32 MaxStackQuantity = 5;
	const FItemData NewItem = FItemData::NewItem
	(
		"TestItem",
		AItemBase::StaticClass(),
		FInventory2D(1, 1),
		ItemQuantity,
		MaxStackQuantity,
		true,
		1.f
	);

	constexpr int32 ItemsToAdd = 3;
	for (int i = 0; i < ItemsToAdd; ++i)
	{
		InventoryComponent->AutoAddItem(NewItem);
	}

	//Inventory should have 2 items tacks
	TestEqual(TEXT("Inventory does not have the right amount of stacks"),InventoryComponent->GetItemCount(),2);
	
	//Inventory should have 10 total quantity of class
	TestEqual(
		TEXT("Item quantity is unexpected"), InventoryComponent->GetTotalCountOfItemClass(AItemBase::StaticClass()),
		ItemQuantity * ItemsToAdd);

	//Inventory Weight should be correct
	TestEqual(TEXT("Inventory weight is unexpected"),
		InventoryComponent->GetInventoryWeight()
		,NewItem.PerItemWeight*ItemsToAdd*ItemQuantity
		);
	
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTransferItem,"Inventory.TransferItem",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FTransferItem::RunTest(const FString& Parameters)
{
	//Create World
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//Create Actor
	AActor* Actor = World->SpawnActor<AActor>();
	TestTrue(TEXT("Test Actor is not valid"), IsValid(Actor));

	//Create and Add Component to Actor
	const FName CompName1("InventoryComp1");
	UInventoryComponent* Inventory1 = NewObject<UInventoryComponent>(Actor, CompName1);
	TestTrue(TEXT("Invetory1 Comp is not valid"), IsValid(Inventory1));

	Inventory1->RegisterComponent();
	TestTrue(TEXT("Inventory1 failed to initialize"), Inventory1->GetSlotCount() > 0);

	//Create and Add Component to Actor
	const FName CompName2("InventoryComp2");
	UInventoryComponent* Inventory2 = NewObject<UInventoryComponent>(Actor, CompName2);
	TestTrue(TEXT("Invetory2 Comp is not valid"), IsValid(Inventory2));

	Inventory2->RegisterComponent();
	TestTrue(TEXT("Inventory2 failed to initialize"), Inventory2->GetSlotCount() > 0);

	
	//Create a valid item to add to inventory
	constexpr int32 ItemQuantity = 2;
	constexpr int32 MaxStackQuantity = 5;
	const FItemData NewItem = FItemData::NewItem
	(
		"TestItem",
		AItemBase::StaticClass(),
		FInventory2D(1, 1),
		ItemQuantity,
		MaxStackQuantity,
		false,
		1.f
	);

	//Add item to inventory
	Inventory1->AutoAddItem(NewItem);

	//Get position of newly added item
	FInventory2D Inv1Position;
	
	if(Inventory1->IsItemInInventory(NewItem,Inv1Position))
	{
		
		const FInventoryItemData TargetItemData = FInventoryItemData(Inv1Position,NewItem);
		const bool bWasTransferOppSuccess = Inventory1->TransferItem(Inventory2,TargetItemData);
		TestTrue(TEXT("Inventory Opperation did not work"),bWasTransferOppSuccess);
	}
	else
	{
		return false;
	}

	return true;
	
}
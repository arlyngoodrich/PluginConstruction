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
	FItemData InvalidItem = FItemData();
	InvalidItem.Invalidate();
	const bool bInvalidItemAdded = InventoryComponent->AddItemToPosition(InvalidItem,FInventory2D(1,1));
	TestTrue(TEXT("Invalid Item Test"),bInvalidItemAdded==false);
	
	//Create a valid item to add to inventory
	const FInventory2D GoodSize = FInventory2D(1,1);
	const FItemData GoodItem = FItemData::NewItem
		(
			"TestItem",
			AItemBase::StaticClass(),
			GoodSize,
			1,
			1,
			false,
			1.f
		);

	const FInventory2D BadSize = FInventory2D(6,1);
	const FItemData BadItem = FItemData::NewItem
	(
		"TestItem",
		AItemBase::StaticClass(),
		BadSize,
		1,
		1,
		false,
		1.f
	);


	const FInventory2D ItemPosition = FInventory2D(1,1);
	
	const bool bBadItemAdded = InventoryComponent->AddItemToPosition(BadItem,ItemPosition);
	TestFalse(TEXT("Bad Item Size Test"),bBadItemAdded);
	

	//Add valid item to inventory
	const bool bValidItemAdded = InventoryComponent->AddItemToPosition(GoodItem,ItemPosition);

	//Double check to make sure it is in the inventory
	FInventory2D ActualItemPosition;
	const bool bDoubleCheckInventory = InventoryComponent->IsItemInInventory(GoodItem,ActualItemPosition);

	//Make sure item has been added and is in correct position
	TestTrue(TEXT("Item Added Check"),bValidItemAdded&&bDoubleCheckInventory);
	TestTrue(TEXT("Expected Position Check"), ActualItemPosition == ItemPosition);
	
	//Make sure weight was added to inventory
	TestEqual(TEXT("Weight was not added to inventory"),
		InventoryComponent->GetInventoryWeight(),
		GoodItem.GetStackWeight());
	
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryMaxWeightTest,"Inventory.MaxWeight",
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWeightSummationTest,"Inventory.WeightSummation",
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
	const FItemData GoodItem = FItemData::NewItem
	(
		"TestItem",
		AItemBase::StaticClass(),
		FInventory2D(1, 1),
		ItemQuantity,
		MaxStackQuantity,
		true,
		1.f
	);
	
	//Add item to inventory
	Inventory1->AutoAddItem(GoodItem);

	//Get position of newly added item
	FInventory2D Inv1Position;
	
	if(Inventory1->IsItemInInventory(GoodItem,Inv1Position))
	{

		//Test Operation
		const FInventoryItemData TargetItemData = FInventoryItemData(Inv1Position,GoodItem);
		const bool bWasTransferOppSuccess = Inventory1->TransferItem(Inventory2,TargetItemData);
		TestTrue(TEXT("Inventory Operation Result"),bWasTransferOppSuccess);

		//Test Item Count
		const bool bInv1Empty = Inventory1->GetItemCount() == 0;
		const bool bInv2HaveItem = Inventory2->GetItemCount() == 1;
		TestTrue(TEXT("Inv1 Empty and Inv2 Has Item"),bInv1Empty && bInv2HaveItem);
		
		//Test Weights
		TestEqual(TEXT("Inv1 weight test"),Inventory1->GetInventoryWeight(),0.f);
		TestEqual(TEXT("Inv2 weight test"),Inventory2->GetInventoryWeight(),GoodItem.GetStackWeight());
		
	}
	else
	{
		return false;
	}
	

	return true;
	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTransferToPosition,"Inventory.TransferToPosition",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FTransferToPosition::RunTest(const FString& Parameters)
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
	const FItemData GoodItem = FItemData::NewItem
	(
		"TestItem",
		AItemBase::StaticClass(),
		FInventory2D(1, 1),
		ItemQuantity,
		MaxStackQuantity,
		true,
		1.f
	);
	
	//Add item to inventory
	Inventory1->AutoAddItem(GoodItem);

	//Get position of newly added item
	FInventory2D Inv1Position;
	
	if(Inventory1->IsItemInInventory(GoodItem,Inv1Position))
	{

		//Test Operation
		const FInventoryItemData TargetItemData = FInventoryItemData(Inv1Position,GoodItem);
		const bool bWasTransferOppSuccess = Inventory1->TransferItemToPosition(Inventory2,FInventory2D(0,0),TargetItemData);
		TestTrue(TEXT("Inventory Operation Result"),bWasTransferOppSuccess);

		//Test Item Count
		const bool bInv1Empty = Inventory1->GetItemCount() == 0;
		const bool bInv2HaveItem = Inventory2->GetItemCount() == 1;
		TestTrue(TEXT("Inv1 Empty and Inv2 Has Item"),bInv1Empty && bInv2HaveItem);
		
		//Test Weights
		TestEqual(TEXT("Inv1 weight test"),Inventory1->GetInventoryWeight(),0.f);
		TestEqual(TEXT("Inv2 weight test"),Inventory2->GetInventoryWeight(),GoodItem.GetStackWeight());
		
	}
	else
	{
		return false;
	}
	

	return true;
	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveItemTest,"Inventory.MoveItem",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMoveItemTest::RunTest(const FString& Parameters)
{
	//Create World
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//Create Actor
	AActor* Actor = World->SpawnActor<AActor>();
	TestTrue(TEXT("Test Actor is not valid"),IsValid(Actor));

	//Create and Add Component to Actor
	const FName CompName("InventoryComp");
	UInventoryComponent* Inventory = NewObject<UInventoryComponent>(Actor,CompName);
	TestTrue(TEXT("Invetory Comp is not valid"),IsValid(Inventory));

	Inventory->RegisterComponent();
	TestTrue(TEXT("Inventory failed to initialize"),Inventory->GetSlotCount()>0);
	
	//Create a valid item to add to inventory
	const FInventory2D Item1Size = FInventory2D(1,2);
	const FItemData Item1 = FItemData::NewItem
		(
			"TestItem1",
			AItemBase::StaticClass(),
			Item1Size,
			1,
			1,
			false,
			1.f
		);

	const FInventory2D ItemPosition = FInventory2D(0,0);
	const bool bItemAdded = Inventory->AddItemToPosition(Item1,ItemPosition);
	TestTrue(TEXT("Item Added Test"),bItemAdded);

	//Bad move test
	const FInventoryItemData InventoryItemData = FInventoryItemData(ItemPosition,Item1);
	const FInventory2D BadMovePosition = FInventory2D(0,1);
	bool bBadMoveResult = Inventory->MoveItem(InventoryItemData,BadMovePosition,false);
	TestFalse(TEXT("Bad Move Test"),bBadMoveResult);

	//Good move test
	const FInventory2D GoodMovePosition = FInventory2D(1,0);
	bool bGoodMoveResult = Inventory->MoveItem(InventoryItemData,GoodMovePosition,false);
	TestTrue(TEXT("Good Move Test"),bGoodMoveResult);

	//Make sure weight is correct
	float ExpectedWeight = Item1.GetStackWeight();
	float ActualWeight = Inventory->GetInventoryWeight();
	TestEqual(TEXT("Weight Check Test"),ActualWeight,ExpectedWeight);

	//Add Second Item to make sure we can't move items 
	const FInventory2D Item2Size = FInventory2D(1,1);
	const FItemData Item2 = FItemData::NewItem
		(
			"TestItem2",
			AItemBase::StaticClass(),
			Item2Size,
			1,
			1,
			false,
			1.f
		);

	FInventory2D Item2Position = FInventory2D(0,0);
	bool bSecondItemAdded = Inventory->AddItemToPosition(Item2,Item2Position);
	TestTrue(TEXT("2nd Item Added Test"),bSecondItemAdded);

	FInventoryItemData InventoryItem2Data = FInventoryItemData(Item2Position,Item2);
	bool bBadSecondItemMoveResult = Inventory->MoveItem(InventoryItem2Data,FInventory2D(1,0),false);
	TestFalse(TEXT("Bad 2nd Item move Test"),bBadSecondItemMoveResult);

	bool bGoodSecondItemMoveResult = Inventory->MoveItem(InventoryItem2Data,FInventory2D(0,1),false);
	TestTrue(TEXT("Good 2nd Item Move Test"),bGoodSecondItemMoveResult);
	
	return true;
	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRotateItemTest,"Inventory.RotateItem",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)


bool FRotateItemTest::RunTest(const FString& Parameters)
{

	//Create World
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//Create Actor
	AActor* Actor = World->SpawnActor<AActor>();
	TestTrue(TEXT("Test Actor is not valid"),IsValid(Actor));

	//Create and Add Component to Actor
	const FName CompName("InventoryComp");
	UInventoryComponent* Inventory = NewObject<UInventoryComponent>(Actor,CompName);
	TestTrue(TEXT("Invetory Comp is not valid"),IsValid(Inventory));

	Inventory->RegisterComponent();
	TestTrue(TEXT("Inventory failed to initialize"),Inventory->GetSlotCount()>0);

	const FItemData Item1 = FItemData::NewItem
		(
			"TestItem1",
			AItemBase::StaticClass(),
			FInventory2D(2,1),
			1,
			1,
			false,
			1.f
		);

	const FItemData Item2 = FItemData::NewItem
		(
			"TestItem1",
			AItemBase::StaticClass(),
			FInventory2D(1,2),
			1,
			1,
			false,
			1.f
		);

	const bool bAddItem1Result = Inventory->AutoAddItem(Item1);
	TestTrue(TEXT("Add Item 1 Test"),bAddItem1Result);

	const bool bAddItem2Results = Inventory->AutoAddItem(Item2);
	TestTrue(TEXT("Add Item 2 Test"),bAddItem2Results);

	FInventoryItemData InventoryItemData;
	Inventory->IsItemInInventory(Item2,InventoryItemData);

	const bool bIsItem2Rotated = InventoryItemData.Item.bIsRotated;
	TestTrue(TEXT("Is Item 2 Rotated"),bIsItem2Rotated);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSplitStackTest,"Inventory.SplitItem",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSplitStackTest::RunTest(const FString& Parameters)
{
	//Create World
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//Create Actor
	AActor* Actor = World->SpawnActor<AActor>();
	TestTrue(TEXT("Test Actor is not valid"),IsValid(Actor));

	//Create and Add Component to Actor
	const FName CompName("InventoryComp");
	UInventoryComponent* Inventory = NewObject<UInventoryComponent>(Actor,CompName);
	TestTrue(TEXT("Invetory Comp is not valid"),IsValid(Inventory));

	Inventory->RegisterComponent();
	TestTrue(TEXT("Inventory failed to initialize"),Inventory->GetSlotCount()>0);

	const FItemData Item1 = FItemData::NewItem
		(
			"TestItem1",
			AItemBase::StaticClass(),
			FInventory2D(1,1),
			10,
			10,
			false,
			0.1f
		);

	const bool bInitialAddItemOperation = Inventory->AddItemToPosition(Item1,FInventory2D(0,0));
	TestTrue(TEXT("Inital Add Item Test"),bInitialAddItemOperation);

	const FInventoryItemData InventoryItemData1 = FInventoryItemData(FInventory2D(0,0),Item1);

	//Make sure splits work
	const bool bFirstSplit = Inventory->SplitItem(InventoryItemData1,1);
	TestTrue(TEXT("First Split Test"),bFirstSplit);

	const bool bSecondSplit = Inventory->SplitItem(InventoryItemData1,1);
	TestTrue(TEXT("Second Split Test"),bSecondSplit);
	
	const bool bThirdSplit = Inventory->SplitItem(InventoryItemData1,1);
	TestTrue(TEXT("Third Split Test"),bThirdSplit);

	const bool bFourthSplit = Inventory->SplitItem(InventoryItemData1,1);
	TestFalse(TEXT("Fourth Split Test"),bFourthSplit);

	//Make sure only 4 items
	const int32 ActualItemQty = Inventory->GetItemCount();
	TestEqual(TEXT("Item Stack Count"),ActualItemQty,4);

	//Make sure weight doesn't change
	const float ActualWeight = Inventory->GetInventoryWeight();
	const float ExpectedWeight = Item1.GetStackWeight();
	TestEqual(TEXT("Inventory Weight"), ActualWeight,ExpectedWeight);

	FGuid Item1Guid = Item1.ItemGuid;
	FInventoryItemData InventoryItemData;

	Inventory->FindInventoryItemAtPosition(FInventory2D(0,1),InventoryItemData);
	FGuid Item2Guid = InventoryItemData.Item.ItemGuid;
	bool bItem2Different = Item1Guid != Item2Guid;
	TestTrue(TEXT("Item2 GUID test"),bItem2Different);

	Inventory->FindInventoryItemAtPosition(FInventory2D(0,1),InventoryItemData);
	FGuid Item3Guid = InventoryItemData.Item.ItemGuid;
	bool bItem3Different = Item1Guid != Item3Guid;
	TestTrue(TEXT("Item3 GUID test"),bItem3Different);

	Inventory->FindInventoryItemAtPosition(FInventory2D(1,1),InventoryItemData);
	FGuid Item4Guid = InventoryItemData.Item.ItemGuid;
	bool bItem4Different = Item1Guid != Item4Guid;
	TestTrue(TEXT("Item4 GUID test"),bItem4Different);
	
	return true;
	
}

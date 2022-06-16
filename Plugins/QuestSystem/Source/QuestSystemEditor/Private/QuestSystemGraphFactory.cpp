// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphFactory.h"
#include "QuestSystemGraph.h"

#define LOCTEXT_NAMESPACE "QuestSystemFactory"

UQuestSystemGraphFactory::UQuestSystemGraphFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UQuestSystemGraph::StaticClass();
}

UObject* UQuestSystemGraphFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UObject>(InParent, InClass, InName, Flags | RF_Transactional);
}

FText UQuestSystemGraphFactory::GetDisplayName() const
{
	return LOCTEXT("FactoryName","Quest System");
}

FString UQuestSystemGraphFactory::GetDefaultNewAssetName() const
{
	return "QuestSystemGraph";
}

#undef LOCTEXT_NAMESPACE
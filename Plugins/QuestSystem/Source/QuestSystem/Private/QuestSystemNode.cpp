// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemNode.h"

#include "QuestSystem.h"
#include "QuestSystemGraph.h"

#define LOCTEXT_NAMESPACE "QuestSystemNode"

UQuestSystemNode::UQuestSystemNode()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UQuestSystemGraph::StaticClass();
	ContextMenuName = LOCTEXT("ConextMenuName", "Quest System Node");
#endif
}

void UQuestSystemNode::OnNodeEntered()
{
	UE_LOG(LogQuestSystem,Log,TEXT("Node entered"))
}

void UQuestSystemNode::OnNodeExit()
{
	UE_LOG(LogQuestSystem,Log,TEXT("Node exited"))
}

#undef LOCTEXT_NAMESPACE

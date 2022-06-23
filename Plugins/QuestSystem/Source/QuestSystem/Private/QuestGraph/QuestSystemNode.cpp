// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraph/QuestSystemNode.h"
#include "QuestGraph/QuestSystem.h"
#include "QuestGraph/QuestSystemGraph.h"

#define LOCTEXT_NAMESPACE "QuestSystemNode"

UQuestSystemNode::UQuestSystemNode()
{

	if(!((QuestSystemGraph = Cast<UQuestSystemGraph>(GetGraph()))))
	{
		UE_LOG(LogQuestSystem,Error,TEXT("%s node is not in a Quest System Graph"),*NodeTitle.ToString())
	}
	
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UQuestSystemGraph::StaticClass();
	ContextMenuName = LOCTEXT("ConextMenuName", "Quest System Node");
#endif
}

void UQuestSystemNode::ActivateNode()
{
	UE_LOG(LogQuestSystem,Log,TEXT("%s node activated"),*NodeTitle.ToString())
}

void UQuestSystemNode::DeactivateNode()
{
	UE_LOG(LogQuestSystem,Log,TEXT("%s node deactivated"),*NodeTitle.ToString())
}

bool UQuestSystemNode::GetNodeIsActive() const {return bIsNodeActive;}


#undef LOCTEXT_NAMESPACE

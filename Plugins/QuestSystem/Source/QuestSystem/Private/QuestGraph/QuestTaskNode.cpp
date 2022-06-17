// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraph/QuestTaskNode.h"

#define LOCTEXT_NAMESPACE "QuestTaskNode"

UQuestTaskNode::UQuestTaskNode()
{

#if WITH_EDITORONLY_DATA

	ContextMenuName = LOCTEXT("ConextMenuName", "Quest Task Node");
	NodeTitle = FText::FromString("Task Node");
	
	ParentLimitType = ENodeLimit::Limited;
	ChildrenLimitType = ENodeLimit::Limited;

	ParentLimit = 1;
	ChildrenLimit = 1;

	BackgroundColor = FLinearColor::Gray;

#endif
}

#undef LOCTEXT_NAMESPACE
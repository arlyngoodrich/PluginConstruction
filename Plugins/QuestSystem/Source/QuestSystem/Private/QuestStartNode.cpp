// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestStartNode.h"

#define LOCTEXT_NAMESPACE "QuestStartNode"

UQuestStartNode::UQuestStartNode()
{

#if WITH_EDITORONLY_DATA

	ContextMenuName = LOCTEXT("ConextMenuName", "Quest Start Node");
	NodeTitle = FText::FromString("Start node");

	ParentLimitType = ENodeLimit::Limited;
	ChildrenLimitType = ENodeLimit::Limited;

	ParentLimit = 0;
	ChildrenLimit = 1;

	BackgroundColor = FLinearColor::Green;

#endif
	
}

#if WITH_EDITOR

bool UQuestStartNode::CanCreateConnectionTo(UGenericGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage)
{

	ErrorMessage = FText::FromString("Cannot connect to Start Nodes");
	
	return false;
}

#endif

#undef LOCTEXT_NAMESPACE

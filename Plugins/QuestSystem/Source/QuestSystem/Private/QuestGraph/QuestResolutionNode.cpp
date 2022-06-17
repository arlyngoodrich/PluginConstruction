// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraph/QuestResolutionNode.h"

#define LOCTEXT_NAMESPACE "QuestResolutionNode"

UQuestResolutionNode::UQuestResolutionNode()
{

#if WITH_EDITORONLY_DATA

	ContextMenuName = LOCTEXT("ConextMenuName", "Quest Resolution Node");
	NodeTitle = FText::FromString("Quest Resolution");
	
	ParentLimitType = ENodeLimit::Unlimited;
	ChildrenLimitType = ENodeLimit::Limited;

	ChildrenLimit = 0;

	BackgroundColor = FLinearColor::Yellow;

#endif
	
}

#if WITH_EDITOR

bool UQuestResolutionNode::CanCreateConnectionFrom(UGenericGraphNode* Other, int32 NumberOfParentNodes,
	FText& ErrorMessage)
{

	ErrorMessage = FText::FromString("Cannot create a connection from a Quest Resolution Node");

	return false;
}

#endif

#undef LOCTEXT_NAMESPACE

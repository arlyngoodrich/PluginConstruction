// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestBranchNode.h"

#define LOCTEXT_NAMESPACE "QuestBranchNode"

UQuestBranchNode::UQuestBranchNode()
{

#if WITH_EDITORONLY_DATA

	ContextMenuName = LOCTEXT("ConextMenuName", "Quest Branch Node");
	NodeTitle = FText::FromString("Branch Node");
	
	ParentLimitType = ENodeLimit::Limited;
	ChildrenLimitType = ENodeLimit::Unlimited;

	ParentLimit = 1;

	BackgroundColor = FLinearColor::Blue;

#endif
}

#if WITH_EDITOR

bool UQuestBranchNode::CanCreateConnectionTo(UGenericGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage)
{

	if(Cast<UQuestBranchNode>(Other))
	{
		ErrorMessage = FText::FromString("Cannot Connect Branch node to Branch Node");
		return false;
	}
	
	return Super::CanCreateConnectionTo(Other, NumberOfChildrenNodes, ErrorMessage);
}

bool UQuestBranchNode::CanCreateConnectionFrom(UGenericGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage)
{

	if(Cast<UQuestBranchNode>(Other))
	{
		ErrorMessage = FText::FromString("Cannot Connect Branch not to Branch Node");
		return false;
	}
	
	return Super::CanCreateConnectionFrom(Other, NumberOfParentNodes, ErrorMessage);
}

#endif

#undef LOCTEXT_NAMESPACE

// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraph/QuestBranchNode.h"

#include "QuestGraph/QuestSystem.h"
#include "QuestGraph/QuestTaskNode.h"

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

void UQuestBranchNode::ActivateNode()
{
	Super::ActivateNode();

	UE_LOG(LogQuestSystem,Log,TEXT("%s has %d children"),*NodeTitle.ToString(),ChildrenNodes.Num());
	
	for (int i = 0; i < ChildrenNodes.Num(); ++i)
	{
		if(UQuestTaskNode* TargetChildTaskNode = Cast<UQuestTaskNode>(ChildrenNodes[i]))
		{
			TargetChildTaskNode->OnTaskNodeCompleted.AddDynamic(this,&UQuestBranchNode::ChildTaskNodeCompleted);
			TargetChildTaskNode->ActivateNode();
		}
		else
		{
			UE_LOG(LogQuestSystem,Error,TEXT("%s should only have task nodes as children"),*NodeTitle.ToString())
		}
		
	}
	
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UQuestBranchNode::ChildTaskNodeCompleted(UQuestTaskNode* CompletedNode)
{
	for (int i = 0; i < ChildrenNodes.Num(); ++i)
	{
		if(UQuestTaskNode* TargetChildTaskNode = Cast<UQuestTaskNode>(ChildrenNodes[i]))
		{
			TargetChildTaskNode->OnTaskNodeCompleted.RemoveDynamic(this,&UQuestBranchNode::ChildTaskNodeCompleted);
			
			if(TargetChildTaskNode != CompletedNode)
			{
				TargetChildTaskNode->CancelNode();
			}
		}
	}

	DeactivateNode();
	
}

#undef LOCTEXT_NAMESPACE

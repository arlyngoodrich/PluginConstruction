// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraph/QuestTaskNode.h"
#include "QuestGraph/QuestSystem.h"
#include "GenericGraphRuntime/Public/GenericGraph.h"
#include "QuestTasks/QuestTaskBase.h"

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

void UQuestTaskNode::ActivateNode()
{
	Super::ActivateNode();

	if(QuestTaskClass == nullptr)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("Quest task %s node in %s does not have valid task class"),
			*NodeTitle.ToString(),*Graph->Name.ToString())
		return;
	}
	
	QuestTask = NewObject<UQuestTaskBase>(QuestTaskClass->GetClass());
	if(QuestTask == nullptr)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("Quest task %s node in %s created a null class"),
			*NodeTitle.ToString(),*Graph->Name.ToString())
		return;
	}

	QuestTask->ActivateTask();
	QuestTask->QuestCompletedDelegate.AddDynamic(this,&UQuestTaskNode::TaskCompleted);
}

void UQuestTaskNode::DeactivateNode()
{
	Super::DeactivateNode();

	QuestTask->QuestCompletedDelegate.RemoveDynamic(this,&UQuestTaskNode::TaskCompleted);
	QuestTask = nullptr;
}

void UQuestTaskNode::TaskCompleted()
{
	if(UQuestSystemNode* ChildNode = Cast<UQuestSystemNode>(ChildrenNodes[0]))
	{
		ChildNode->ActivateNode();
		OnTaskNodeCompleted.Broadcast(this);
		DeactivateNode();
	}
	else
	{
		UE_LOG(LogQuestSystem,Error,TEXT("Start node in %s does not have valid child"),*Graph->Name.ToString())
	}
}

#undef LOCTEXT_NAMESPACE

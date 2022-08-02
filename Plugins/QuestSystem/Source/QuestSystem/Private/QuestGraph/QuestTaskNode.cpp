// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraph/QuestTaskNode.h"
#include "QuestGraph/QuestSystem.h"
#include "GenericGraphRuntime/Public/GenericGraph.h"
#include "QuestGraph/QuestSystemGraph.h"
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

	if(QuestSystemGraph)
	{
		QuestTask = NewObject<UQuestTaskBase>(QuestSystemGraph->WorldRef,QuestTaskClass);
		if(QuestTask == nullptr)
		{
			UE_LOG(LogQuestSystem,Error,TEXT("Quest task %s node in %s created a null class"),
				*NodeTitle.ToString(),*Graph->Name.ToString())
			return;
		}

		UE_LOG(LogQuestSystem,Log,TEXT("%s task node created %s quest task"),*NodeTitle.ToString(),*QuestTask->GetName())

		QuestTask->QuestCompletedDelegate.AddDynamic(this,&UQuestTaskNode::TaskCompleted);
		QuestSystemGraph->CheckInTaskNode(this);
		QuestTask->ActivateTask(QuestSystemGraph->WorldRef);
	}
	else
	{
		UE_LOG(LogQuestSystem,Error,TEXT("%s node is not in quest graph"),*NodeTitle.ToString())
	}

	
}

void UQuestTaskNode::DeactivateNode()
{
	Super::DeactivateNode();

	QuestSystemGraph->CheckOutTaskNode(this);
	QuestTask->QuestCompletedDelegate.RemoveDynamic(this,&UQuestTaskNode::TaskCompleted);
	QuestTask->MarkAsGarbage();
	QuestTask = nullptr;
	
}

void UQuestTaskNode::CancelNode()
{
	QuestTask->CancelTask();
	DeactivateNode();
}

UQuestTaskBase* UQuestTaskNode::GetQuestTask(){ return QuestTask;}

void UQuestTaskNode::TaskCompleted()
{
	UE_LOG(LogQuestSystem,Log,TEXT("%s task completed"),*NodeTitle.ToString())
	
	if(UQuestSystemNode* ChildNode = Cast<UQuestSystemNode>(ChildrenNodes[0]))
	{
		OnTaskNodeCompleted.Broadcast(this);
		DeactivateNode();
		ChildNode->ActivateNode();
	}
	else
	{
		UE_LOG(LogQuestSystem,Error,TEXT("Task node %s in %s does not have valid child"),*NodeTitle.ToString(),*Graph->Name.ToString())
	}
}

#undef LOCTEXT_NAMESPACE

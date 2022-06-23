// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraph/QuestSystemGraph.h"

#include "GameFramework/Character.h"
#include "QuestGraph/QuestBranchNode.h"
#include "QuestGraph/QuestResolutionNode.h"
#include "QuestGraph/QuestStartNode.h"
#include "QuestGraph/QuestSystem.h"
#include "QuestGraph/QuestSystemGraphEdge.h"
#include "QuestGraph/QuestSystemNode.h"
#include "QuestGraph/QuestTaskNode.h"

UQuestSystemGraph::UQuestSystemGraph()
{
	NodeType = UQuestSystemNode::StaticClass();
	EdgeType = UQuestSystemGraphEdge::StaticClass();
	bEdgeEnabled = true;

#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;
	bCanRenameNode = true;
#endif
}

void UQuestSystemGraph::InitializeQuest()
{
	QuestInfo.QuestGUID = FGuid::NewGuid();
	QuestInfo.QuestName = Name;
	QuestInfo.QuestDescription = QuestDescription;
}

void UQuestSystemGraph::StartQuest(APlayerController* SetInstigatingPlayer)
{
	UE_LOG(LogQuestSystem,Log,TEXT("Starting %s quest"),*Name.ToString())

	if(EnsureGraphStructure() == false)
	{
		return;
	}

	UQuestStartNode* StartNode = GetStartNode();
	if(StartNode == nullptr)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("Graph %s start node is null"),*Name.ToString())
		return;
	}

	if(SetInstigatingPlayer == nullptr)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("%s quest graph needs valid instigating player"),*Name.ToString())
		return;
	}
	InstigatingPlayer = SetInstigatingPlayer;
		
	WorldRef = InstigatingPlayer->GetWorld();
	if(WorldRef == nullptr)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("%s quest graph needs valid referenc to world"),*Name.ToString())
		return;
	}

	StartNode->ActivateNode();
}

UQuestStartNode* UQuestSystemGraph::GetStartNode()
{
	//Make sure start node
	TArray<UGenericGraphNode*> BaseNodes;
	GetNodesByLevel(0,BaseNodes);

	return Cast<UQuestStartNode>(BaseNodes[0]);
}

TArray<UQuestTaskBase*> UQuestSystemGraph::GetActiveTasks()
{
	TArray<UQuestTaskBase*> TaskBases;
	for (int i = 0; i < ActiveTaskNodes.Num(); ++i)
	{
		TaskBases.Add(ActiveTaskNodes[i]->GetQuestTask());
	}

	return TaskBases;
}



void UQuestSystemGraph::CheckInTaskNode(UQuestTaskNode* TaskNode)
{
	if(TaskNode == nullptr)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("%s cannot add null task node to active task array"),*Name.ToString())
		return;
	}

	if(ActiveTaskNodes.Contains(TaskNode))
	{
		UE_LOG(LogQuestSystem,Warning,TEXT("%s is already in %s active task array. Cannot add again."),
			*TaskNode->NodeTitle.ToString(),*Name.ToString())
		return;
	}

	ActiveTaskNodes.Add(TaskNode);
	OnActiveTasksUpdate.Broadcast(GetActiveTasks());
}

void UQuestSystemGraph::CheckOutTaskNode(UQuestTaskNode* TaskNode)
{
	if(TaskNode == nullptr)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("%s cannot remove nmull task node from Active Task array"),*Name.ToString())
		return;
	}

	if(ActiveTaskNodes.Contains(TaskNode))
	{
		ActiveTaskNodes.Remove(TaskNode);
		OnActiveTasksUpdate.Broadcast(GetActiveTasks());
	}
	else
	{
		UE_LOG(LogQuestSystem,Warning,TEXT("%s is not part of %s active task array and cannot be removed"),
			*TaskNode->NodeTitle.ToString(),*Name.ToString())
	}
}

bool UQuestSystemGraph::EnsureGraphStructure()
{

	TArray<bool> GraphChecks;
	GraphChecks.Add(true);
	
	//Make sure start node
	TArray<UGenericGraphNode*> BaseNodes;
	GetNodesByLevel(0,BaseNodes);

	//Check if more than one root node
	if(BaseNodes.Num()!=1)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("Quest %s should have one root node"),*Name.ToString())
		GraphChecks.Add(false);
	}
	//If only one root node, make sure it's a start node
	else if(!Cast<UQuestStartNode>(BaseNodes[0]))
	{
		UE_LOG(LogQuestSystem,Error,TEXT("Quest %s root node is not a start node"),*Name.ToString())
		GraphChecks.Add(false);
	}

	//Make sure at least one Resolution node
	int32 ResolutionNodes = 0;
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		if(Cast<UQuestResolutionNode>(AllNodes[i]))
		{
			ResolutionNodes ++;
		}
		
		//If task node, make sure to have more than one parent and child
		else if(const UQuestTaskNode* TaskNode= Cast<UQuestTaskNode>(AllNodes[i]))
		{
			if(TaskNode->ParentNodes.Num() == 0 || TaskNode->ChildrenNodes.Num() == 0)
			{
				UE_LOG(LogQuestSystem,Log,TEXT("Graph %s task node %s does not have at least one parent or child"),
					*Name.ToString(),*TaskNode->NodeTitle.ToString())
				GraphChecks.Add(false);
			}
		}
		//If resolution node, make sure have more than one parent and child
		else if(const UQuestBranchNode* BranchNode = Cast<UQuestBranchNode>(AllNodes[i]))
		{
			if(BranchNode->ParentNodes.Num() == 0 || BranchNode->ChildrenNodes.Num() == 0)
			{
				UE_LOG(LogQuestSystem,Log,TEXT("Graph %s branch node %s does not have at least one parent or child"),
					*Name.ToString(),*TaskNode->NodeTitle.ToString())
				GraphChecks.Add(false);
			}
		}
	}

	//Make sure branches have at least one parent and one child
	if(ResolutionNodes == 0)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("Quest %s should have at least one resolution node"),*Name.ToString());
		GraphChecks.Add(false);
	}
		
	return !GraphChecks.Contains(false);
}


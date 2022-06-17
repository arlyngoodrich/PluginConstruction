// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraph/QuestSystemGraph.h"
#include "QuestGraph/QuestSystemGraphEdge.h"
#include "QuestGraph/QuestSystemNode.h"

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

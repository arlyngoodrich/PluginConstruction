// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericGraph.h"
#include "QuestSystemGraph.generated.h"

class UQuestStartNode;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestSystemGraph : public UGenericGraph
{
	GENERATED_BODY()

public:

	UQuestSystemGraph();

	UFUNCTION(BlueprintCallable,Category="Quest System")
	void StartQuest();

	UFUNCTION(BlueprintCallable,Category="Quest System")
	UQuestStartNode* GetStartNode();
	
protected:

	/**
	 * @brief Checks to make sure graph structure is correct.  Graph must have a start node, at least one resolution node,
	 * tasks and branches must have at least one parent and child nodes
	 * @return True if graph is OK, false if not.
	 */
	bool EnsureGraphStructure();
	
	
};

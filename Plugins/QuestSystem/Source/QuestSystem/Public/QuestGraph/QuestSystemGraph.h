// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericGraph.h"
#include "QuestData.h"
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

	UPROPERTY(BlueprintReadOnly,Category="Quest System")
	FQuestInfo QuestInfo;

	/**
	 * @brief Starts quest by activating start node
	 * @param SetInstigatingPlayer Player that is starting the quest
	 */
	UFUNCTION(BlueprintCallable,Category="Quest System")
	void StartQuest(APlayerController* SetInstigatingPlayer);

	/**
	 * @brief Gets start node for quest
	 * @return Pointer to start node of quest
	 */
	UFUNCTION(BlueprintCallable,Category="Quest System")
	UQuestStartNode* GetStartNode();

	

#if WITH_EDITORONLY_DATA
	/**
	 * @brief Description of quest with GUID created on construction
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Quest System")
	FText QuestDescription;

#endif
	

	UPROPERTY()
	APlayerController* InstigatingPlayer;
	
	UPROPERTY()
	UWorld* WorldRef;
	
protected:

	/**
	 * @brief Checks to make sure graph structure is correct.  Graph must have a start node, at least one resolution node,
	 * tasks and branches must have at least one parent and child nodes
	 * @return True if graph is OK, false if not.
	 */
	bool EnsureGraphStructure();
	
	
};

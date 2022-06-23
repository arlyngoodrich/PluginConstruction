// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericGraph.h"
#include "QuestData.h"
#include "QuestSystemGraph.generated.h"

class UQuestStartNode;
class UQuestTaskNode;
class UQuestTaskBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveTasksUpdate,TArray<UQuestTaskBase*>, QuestTasks);


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

	UPROPERTY(BlueprintAssignable,Category="Quest System")
	FOnActiveTasksUpdate OnActiveTasksUpdate;

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

	UFUNCTION(BlueprintCallable,Category="Quest Sytem")
	TArray<UQuestTaskBase*> GetActiveTasks();

	/**
	 * @brief Initializes quest info but does not activate quest
	 */
	UFUNCTION(BlueprintCallable,Category="Quest System")
	void InitializeQuest();

	/**
	 * @brief Ensures task is not null or already in array, will then add to active task array
	 * @param TaskNode Task node to add to Active Tasks array
	 */
	void CheckInTaskNode(UQuestTaskNode* TaskNode);

	/**
	 * @brief Ensures task node is alrady in array then removes from active task array 
	 * @param TaskNode task node pointer to remove from array 
	 */
	void CheckOutTaskNode(UQuestTaskNode* TaskNode);




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

	/**
	 * @brief Pointers to active task nodes.  Task nodes 'check in' when they are activated and 'check out' when they
	 * are deactivated. 
	 */
	UPROPERTY(BlueprintReadOnly,Category="Quest System")
	TArray<UQuestTaskNode*> ActiveTaskNodes;
	
	
};


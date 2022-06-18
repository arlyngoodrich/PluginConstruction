// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemNode.h"
#include "QuestTaskNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskNodeCompleted, class UQuestTaskNode*, TaskNode);

class UQuestTaskBase;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestTaskNode : public UQuestSystemNode
{
	GENERATED_BODY()

public:
	UQuestTaskNode();

	/**
	 * @brief Quest task to be activated by this node
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Quest System")
	TSubclassOf<UQuestTaskBase> QuestTaskClass;

	UPROPERTY(BlueprintAssignable,Category="Quest System")
	FTaskNodeCompleted OnTaskNodeCompleted;

	virtual void ActivateNode() override;

	virtual void DeactivateNode() override;

	virtual void CancelNode();

protected:

	UPROPERTY(BlueprintReadOnly,Category="Quest System")
	UQuestTaskBase* QuestTask;

	UFUNCTION()
	void TaskCompleted();
};

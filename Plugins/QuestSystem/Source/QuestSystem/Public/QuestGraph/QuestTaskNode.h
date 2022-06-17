// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemNode.h"
#include "QuestTaskNode.generated.h"

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
	TSubclassOf<UQuestTaskBase> QuestTask;
	
};

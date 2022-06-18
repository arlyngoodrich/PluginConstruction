// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestTaskBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestCompleted);

/**
 * 
 */


UCLASS(Blueprintable)
class QUESTSYSTEM_API UQuestTaskBase : public UObject
{
	GENERATED_BODY()


public:

	/**
	 * @brief Short UI title for quest
	 */
	UPROPERTY(BlueprintReadOnly, Category="Quest System")
	FText QuestTitle;

	/**
	 * @brief Long description of quest
	 */
	UPROPERTY(BlueprintReadOnly, Category="Quest System")
	FText QuestDescription;

	UPROPERTY(BlueprintAssignable,Category="Quest System")
	FOnQuestCompleted QuestCompletedDelegate;
	
	/**
	 * @brief Should be called by object creating quest to kick off quest logic. 
	 */
	UFUNCTION(BlueprintCallable,Category="Quest System")
	virtual void ActivateTask(UWorld* World);

	/**
	 * @brief Called when task is activated
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Quest System")
	void TaskActivated();

	/**
	 * @brief Called when quest is completed.  Broadcasts OnQuestCompleted delegates
	 */
	UFUNCTION(BlueprintCallable,Category="Quest System")
	virtual void CompleteTask();

protected:

	/**
	 * @brief Overridable internal version of Task Activated
	 */
	virtual void Internal_TaskActivated();

	UPROPERTY()
	UWorld* WorldRef;

	
};

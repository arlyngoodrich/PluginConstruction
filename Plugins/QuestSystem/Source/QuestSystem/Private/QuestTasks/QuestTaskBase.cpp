// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTasks/QuestTaskBase.h"

#include "QuestGraph/QuestSystem.h"

void UQuestTaskBase::ActivateTask(UWorld* World)
{
	if(World == nullptr)
	{
		UE_LOG(LogQuestSystem,Log,TEXT("%s needs valid world ref to activate"),*GetName())
		return;
	}

	WorldRef = World;
	
	TaskActivated();
	UE_LOG(LogQuestSystem,Log,TEXT("%s quest task activated"),*GetName())
}

void UQuestTaskBase::TaskActivated_Implementation()
{
	Internal_TaskActivated();
}


void UQuestTaskBase::CompleteTask()
{
	UE_LOG(LogQuestSystem,Log,TEXT("%s quest task completed"),*GetName())
	QuestCompletedDelegate.Broadcast();
}

void UQuestTaskBase::Internal_TaskActivated()
{
	
}

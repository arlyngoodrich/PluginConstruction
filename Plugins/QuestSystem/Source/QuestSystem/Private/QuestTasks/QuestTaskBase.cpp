// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTasks/QuestTaskBase.h"

void UQuestTaskBase::ActivateTask()
{
	TaskActivated();
}

void UQuestTaskBase::TaskActivated_Implementation()
{
	Internal_TaskActivated();
}

void UQuestTaskBase::CompleteTask()
{
	QuestCompletedDelegate.Broadcast();
}

void UQuestTaskBase::Internal_TaskActivated()
{
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTasks/QuestTaskBase.h"

void UQuestTaskBase::ActivateQuest()
{
}

void UQuestTaskBase::CompletedQuest()
{
	QuestCompletedDelegate.Broadcast();
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerQuestManager.h"
#include "QuestGraph/QuestSystem.h"
#include "QuestGraph/QuestSystemGraph.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerQuestManager::UPlayerQuestManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
	
	// ...
}




// Called when the game starts
void UPlayerQuestManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



void UPlayerQuestManager::GetLifetimeReplicatedProps (TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerQuestManager,ActiveQuest);
	DOREPLIFETIME(UPlayerQuestManager,CompletedQuests);
	DOREPLIFETIME(UPlayerQuestManager,AvailableQuests);
	
}


void UPlayerQuestManager::AddNewQuest(UQuestSystemGraph* NewQuest)
{
	if(NewQuest == nullptr)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("%s quest manager received null quest"),*GetOwner()->GetName())
		return;
	}

	if(AvailableQuests.Contains(NewQuest))
	{
		UE_LOG(LogQuestSystem,Warning,TEXT("%s quest manager already contains %s quest. Cannot add duplicate"),
			*GetOwner()->GetName(),*NewQuest->Name.ToString())
		return;
	}

	if(GetOwner()->HasAuthority() == false)
	{
		Server_AddNewQuest(NewQuest);
		return;
	}

	AvailableQuests.Add(NewQuest);
	UE_LOG(LogQuestSystem,Log,TEXT("%s added %s quest"),*GetOwner()->GetName(),*NewQuest->Name.ToString());
}

void UPlayerQuestManager::SetActiveQuest(UQuestSystemGraph* Quest)
{
	if(Quest == nullptr)
	{
		UE_LOG(LogQuestSystem,Error,TEXT("%s quest manager received null quest to make active"),
			*GetOwner()->GetName())
		return;
	}

	if(AvailableQuests.Contains(Quest))
	{

		if(GetOwner()->HasAuthority() == false)
		{
			Server_SetActiveQuest(Quest);
			return;
		}
		
		ActiveQuest = Quest;
		OnRep_ActiveQuestUpdated();
		UE_LOG(LogQuestSystem,Log,TEXT("%s activated %s quest"),*GetOwner()->GetName(),*Quest->Name.ToString());
	}
	else
	{
		UE_LOG(LogQuestSystem,Warning,TEXT("%s quest manager does not contains %s quest. Cannot make active"),
			*GetOwner()->GetName(),*Quest->Name.ToString())
	}
	
}

void UPlayerQuestManager::OnRep_ActiveQuestUpdated()
{
	OnActiveQuestUpdated.Broadcast(ActiveQuest->QuestInfo);
}

bool UPlayerQuestManager::Server_SetActiveQuest_Validate(UQuestSystemGraph* Quest)
{
	return true;
}

void UPlayerQuestManager::Server_SetActiveQuest_Implementation(UQuestSystemGraph* Quest)
{
	SetActiveQuest(Quest);
}

bool UPlayerQuestManager::Server_AddNewQuest_Validate(UQuestSystemGraph* NewQuest)
{
	return true;
}

void UPlayerQuestManager::Server_AddNewQuest_Implementation(UQuestSystemGraph* NewQuest)
{
	AddNewQuest(NewQuest);
}
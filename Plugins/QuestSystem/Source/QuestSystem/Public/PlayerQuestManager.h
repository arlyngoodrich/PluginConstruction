// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestData.h"
#include "PlayerQuestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveQuestUpdated,FQuestInfo,NewQuestInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestTaskUpdated,TArray<FQuestTaskInfo>, TaskInfo);


class UQuestSystemGraph;
class UQuestTaskBase;


UCLASS( ClassGroup=(QuestSystem),Blueprintable,meta=(BlueprintSpawnableComponent) )
class QUESTSYSTEM_API UPlayerQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerQuestManager();

	UPROPERTY(BlueprintAssignable,Category="Quest System")
	FOnActiveQuestUpdated OnActiveQuestUpdated;

	UPROPERTY(BlueprintAssignable,Category="Quest System")
	FOnQuestTaskUpdated OnQuestTaskUpdated;

	/**
	 * @brief Adds a new quest to the available quests array.  Checks for duplicates. 
	 * @param NewQuest New quest to add to available quest array 
	 */
	UFUNCTION(BlueprintCallable,Category="Quest System")
	void AddNewQuest(UQuestSystemGraph* NewQuest);

	/**
	 * @brief Selects quest from available quest array ane makes it active.  Quest must be available to become the active
	 * quest
	 * @param Quest Quest to make active 
	 */
	UFUNCTION(BlueprintCallable,Category="Quest System")
	void SetActiveQuest(UQuestSystemGraph* Quest);

	/**
	 * @brief Used to update Quest info in UI
	 */
	UFUNCTION(BlueprintImplementableEvent,Category="Quest System")
	void OnActiveQuestSet();

	/**
	 * @brief Bound to active quest, called when quest graph is resolved
	 */
	UFUNCTION(Category="Quest System")
	void ResolveQuest(UQuestSystemGraph* Quest);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * @brief Array of tracked quest pointers 
	 */
	UPROPERTY(BlueprintReadOnly,Replicated, Category="Quest System")
	TArray<UQuestSystemGraph*> AvailableQuests;

	/**
	 * @brief Array of pointers to completed quests
	 */
	UPROPERTY(BlueprintReadOnly,Replicated, Category="Quest System")
	TArray<UQuestSystemGraph*> CompletedQuests;

	/**
	 * @brief Pointer to the currently active quest
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ActiveQuestUpdated,Category="Quest System")
	UQuestSystemGraph* ActiveQuest;

	UPROPERTY(BlueprintReadOnly,Replicated,Category="Quest System")
	TArray<UQuestTaskBase*> ActiveTasks;

	UFUNCTION()
	void OnRep_ActiveQuestUpdated() const;

	UFUNCTION()
	void OnRep_TasksUpdated();

	UFUNCTION()
	void ListenForTasksUpdated(TArray<UQuestTaskBase*> QuestTasks);

	/**
	 * @brief RPC for clients to set active quest
	 */
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_SetActiveQuest(UQuestSystemGraph* Quest);
	bool Server_SetActiveQuest_Validate(UQuestSystemGraph* Quest);
	void Server_SetActiveQuest_Implementation(UQuestSystemGraph* Quest);

	/**
	 * @brief RPC for clients to add a new quest
	 */
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_AddNewQuest(UQuestSystemGraph* NewQuest);
	bool Server_AddNewQuest_Validate(UQuestSystemGraph* NewQuest);
	void Server_AddNewQuest_Implementation(UQuestSystemGraph* NewQuest);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_ResolveQuest(UQuestSystemGraph* ResolvedQuest);
	bool Server_ResolveQuest_Validate(UQuestSystemGraph* ResolvedQuest);
	void Server_ResolveQuest_Implementation(UQuestSystemGraph* ResolvedQuest);
	
	
};

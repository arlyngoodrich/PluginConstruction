// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerQuestManager.generated.h"

class UQuestSystemGraph;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUESTSYSTEM_API UPlayerQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerQuestManager();

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
	UPROPERTY(BlueprintReadOnly Replicated,Category="Quest System")
	UQuestSystemGraph* ActiveQuest;

};

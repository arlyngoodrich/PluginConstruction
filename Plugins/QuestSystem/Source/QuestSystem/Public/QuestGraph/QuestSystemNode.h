// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "QuestSystemNode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class QUESTSYSTEM_API UQuestSystemNode : public UGenericGraphNode
{
	GENERATED_BODY()
	
public:
	UQuestSystemNode();

	virtual void ActivateNode();

	virtual void DeactivateNode();

	UFUNCTION(BlueprintCallable,Category="Quest System")
	bool GetNodeIsActive() const;

protected:
	UPROPERTY(BlueprintReadOnly,Category="Quest System")
	bool bIsNodeActive;
	
};

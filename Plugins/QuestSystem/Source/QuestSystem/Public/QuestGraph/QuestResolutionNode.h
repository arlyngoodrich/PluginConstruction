// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemNode.h"
#include "UObject/Object.h"
#include "QuestResolutionNode.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestResolutionNode : public UQuestSystemNode
{
	GENERATED_BODY()

public:
	
	UQuestResolutionNode();

#if WITH_EDITOR

	virtual bool CanCreateConnection(UGenericGraphNode* Other, FText& ErrorMessage) override;

#endif


	virtual void ActivateNode() override;
		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemNode.h"
#include "UObject/Object.h"
#include "QuestStartNode.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestStartNode : public UQuestSystemNode
{
	GENERATED_BODY()

	UQuestStartNode();

#if WITH_EDITOR

	virtual bool CanCreateConnectionTo(UGenericGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage) override;


#endif
	
};

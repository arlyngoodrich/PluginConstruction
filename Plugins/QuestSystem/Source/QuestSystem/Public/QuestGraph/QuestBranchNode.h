// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemNode.h"
#include "UObject/Object.h"
#include "QuestBranchNode.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestBranchNode : public UQuestSystemNode
{
	GENERATED_BODY()

public:
	
	UQuestBranchNode();

#if WITH_EDITOR

	virtual bool CanCreateConnectionTo(UGenericGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage) override;
	
	virtual bool CanCreateConnectionFrom(UGenericGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage) override;

#endif
	
	
};

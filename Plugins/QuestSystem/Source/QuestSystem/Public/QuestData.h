// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestData.generated.h"

/**
 * @brief Data Struct for Inventory Slots
 */
USTRUCT(BlueprintType)
struct FQuestInfo 
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest System")
	FText QuestName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest System")
	FText QuestDescription;

	UPROPERTY(BlueprintReadOnly, Category = "Quest System")
	FGuid QuestGUID;
	

	/**
	 * @brief Default constructor. Sets texts to none and guid to invalid
	 */
	FQuestInfo()
	{
		QuestName = FText::GetEmpty();
		QuestDescription = FText::GetEmpty();
		QuestGUID.Invalidate();
	}

	
	/**
	 * @brief Creates new Quest Info data struct.  Creates new GUID
	 * @param SetQuestName Quest Name
	 * @param SetQuestDescription Quest Description
	 */
	FQuestInfo(const FText SetQuestName, const FText SetQuestDescription)
	{
		QuestName = SetQuestName;
		QuestDescription = SetQuestDescription;
		QuestGUID = FGuid::NewGuid();
	}

	/**
 * @brief Creates new Quest Info data struct
 * @param SetQuestName Quest Name
 * @param SetQuestDescription Quest Description
 * @param SetQuestGUID sets new GUID
 */
	FQuestInfo(const FText SetQuestName, const FText SetQuestDescription, const FGuid SetQuestGUID)
	{
		QuestName = SetQuestName;
		QuestDescription = SetQuestDescription;
		QuestGUID = SetQuestGUID;
	}

	bool operator==(const FQuestInfo& QuestInfo) const
	{
		return QuestGUID == (QuestInfo.QuestGUID) && QuestGUID.IsValid();
	}
	
	
};
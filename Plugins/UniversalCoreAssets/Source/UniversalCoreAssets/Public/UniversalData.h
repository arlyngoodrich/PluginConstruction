// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UniversalData.generated.h"

UENUM(BlueprintType)
enum class EUniversalAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Sprint,
	Jump
};
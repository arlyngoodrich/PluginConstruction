// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginConstructionGameMode.h"
#include "PluginConstructionCharacter.h"
#include "UObject/ConstructorHelpers.h"

APluginConstructionGameMode::APluginConstructionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCharacter/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
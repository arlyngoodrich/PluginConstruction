// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


DECLARE_LOG_CATEGORY_EXTERN(LogUniversalCoreAssets, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogUniversalAbilitySystem, Log, All);

class FUniversalCoreAssetsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

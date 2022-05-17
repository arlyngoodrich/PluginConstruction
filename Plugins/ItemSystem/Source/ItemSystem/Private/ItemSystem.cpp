// Copyright Epic Games, Inc. All Rights Reserved.

#include "ItemSystem.h"


#define LOCTEXT_NAMESPACE "FItemSystemModule"

DEFINE_LOG_CATEGORY(LogItemSystem)
DEFINE_LOG_CATEGORY(LogTestItemSystem)


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FItemSystemModule, ItemSystem)

void FItemSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FItemSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



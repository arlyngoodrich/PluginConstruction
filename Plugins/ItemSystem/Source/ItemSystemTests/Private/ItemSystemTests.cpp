#include "ItemSystemTests.h"

#define LOCTEXT_NAMESPACE "FItemSystemTestsModule"

#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(FItemSystemTestsModule, ItemSystemTests)

void FItemSystemTestsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FItemSystemTestsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}
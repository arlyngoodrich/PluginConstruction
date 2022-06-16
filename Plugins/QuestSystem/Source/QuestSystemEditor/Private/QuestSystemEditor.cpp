// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystemEditor.h"
#include "Logging.h"

#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditor"

void FQuestSystemEditor::StartupModule()
{
}

void FQuestSystemEditor::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestSystemEditor, QuestSystemEditor);
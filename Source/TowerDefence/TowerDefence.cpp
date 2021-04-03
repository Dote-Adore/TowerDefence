// Copyright Epic Games, Inc. All Rights Reserved.

#include "TowerDefence.h"

#include "GlobalConfig.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FMyGameModule, TowerDefence, "TowerDefence" );

void FMyGameModule::StartupModule()
{
    FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").RegisterSettings(
        "Project",
        "Game",
        TEXT("TDGlobalSettings"),
        FText::FromString("Tower Defence Global Settings"),
        FText::FromString("Settings for Tower Defence Game"),
        GetMutableDefault<UGlobalConfig>());
}

void FMyGameModule::ShutdownModule()
{
    FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").UnregisterSettings("Project", "Game", "TDGlobalSettings");
}

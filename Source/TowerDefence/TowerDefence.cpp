// Copyright Epic Games, Inc. All Rights Reserved.

#include "TowerDefence.h"

#include "GlobalConfig.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ToolMenus.h"
#include "Modules/ModuleManager.h"
#endif

IMPLEMENT_PRIMARY_GAME_MODULE(FMyGameModule, TowerDefence, "TowerDefence");
#define LOCTEXT_NAMESPACE "FLevelEditor"
void FMyGameModule::StartupModule()
{      // 注册设置
#if WITH_EDITOR
    FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").RegisterSettings(
        "Project",
        "Game",
        TEXT("TDGlobalSettings"),
        FText::FromString("Tower Defence Global Settings"),
        FText::FromString("Settings for Tower Defence Game"),
        GetMutableDefault<UGlobalConfig>());
#endif
}

void FMyGameModule::ShutdownModule()
{
#if WITH_EDITOR
    FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").UnregisterSettings("Project", "Game", "TDGlobalSettings");
#endif
}


#undef LOCTEXT_NAMESPACE
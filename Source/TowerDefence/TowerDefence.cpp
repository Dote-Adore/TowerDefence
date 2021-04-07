// Copyright Epic Games, Inc. All Rights Reserved.

#include "TowerDefence.h"

#include "GlobalConfig.h"
#include "ISettingsModule.h"
#include "ToolMenus.h"
#include "Interfaces/IPluginManager.h"
#include "LevelEditor/SLevelCreatorPanel.h"
#include "TowerDefence/LevelEditor/LevelEditorCommands.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"
#include "Widgets/Docking/SDockTab.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FMyGameModule, TowerDefence, "TowerDefence");
#define LOCTEXT_NAMESPACE "FLevelEditor"
FName FMyGameModule::LevelCreatorTab = "LevelCreatorTab";

void FMyGameModule::StartupModule()
{      // 注册设置
    FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").RegisterSettings(
        "Project",
        "Game",
        TEXT("TDGlobalSettings"),
        FText::FromString("Tower Defence Global Settings"),
        FText::FromString("Settings for Tower Defence Game"),
        GetMutableDefault<UGlobalConfig>());

    // LevelCreator插件注册
    FLevelEditorCommands::Register();
    PluginCommands = MakeShareable(new FUICommandList);
    PluginCommands->MapAction(
        FLevelEditorCommands::Get().LevelCreatorAction,
        FExecuteAction::CreateRaw(this, &FMyGameModule::OnLevelCreatorButtonClicked),
        FCanExecuteAction());
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMyGameModule::RegisterMenus));

    // Tab注册
    // TabManager
    FGlobalTabmanager::Get()->RegisterTabSpawner(LevelCreatorTab,
        FOnSpawnTab::CreateRaw(this, &FMyGameModule::OnSpawnLevelCreatorTab))
    .SetDisplayName(LOCTEXT("LevelCreatorEditor","Level Creator Editor"))
    .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMyGameModule::ShutdownModule()
{
    FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").UnregisterSettings("Project", "Game", "TDGlobalSettings");

    FGlobalTabmanager::Get()->UnregisterTabSpawner(LevelCreatorTab);
    
}

void FMyGameModule::RegisterMenus()
{
    // Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
    FToolMenuOwnerScoped OwnerScoped(this);
    UToolMenus* ToolMenus = UToolMenus::Get();
    UToolMenu* Menu = ToolMenus->ExtendMenu("LevelEditor.LevelEditorToolBar");
    FToolMenuSection& Section = Menu->FindOrAddSection("Settings");
    {
          FToolMenuEntry& Entry = Section.AddEntry(
              FToolMenuEntry::InitToolBarButton(FLevelEditorCommands::Get().LevelCreatorAction));
         Entry.SetCommandList(PluginCommands);
    }
}

void FMyGameModule::OnLevelCreatorButtonClicked()
{
    FGlobalTabmanager::Get()->TryInvokeTab(LevelCreatorTab);
}

TSharedRef<SDockTab> FMyGameModule::OnSpawnLevelCreatorTab(const FSpawnTabArgs& SpawnTabArgs)
{
    return SNew(SDockTab)
    .Label(FText::FromString("Level Creator"))
    [
        SNew(SLevelCreatorPanel)
    ];
    
}


#undef LOCTEXT_NAMESPACE
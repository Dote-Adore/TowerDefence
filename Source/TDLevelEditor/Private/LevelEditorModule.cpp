#include "LevelEditorModule.h"
// Copyright Epic Games, Inc. All Rights Reserved.
#include "ISettingsModule.h"
#include "ToolMenus.h"
#include "Interfaces/IPluginManager.h"
#include "SLevelCreatorPanel.h"
#include "LevelEditorCommands.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "FTDLevelEditor"
FName FTDLevelEditorModule::LevelCreatorTab = "LevelCreatorTab";

void FTDLevelEditorModule::StartupModule()
{

    // LevelCreator插件注册
    FLevelEditorCommands::Register();
    PluginCommands = MakeShareable(new FUICommandList);
    PluginCommands->MapAction(
        FLevelEditorCommands::Get().LevelCreatorAction,
        FExecuteAction::CreateRaw(this, &FTDLevelEditorModule::OnLevelCreatorButtonClicked),
        FCanExecuteAction());
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTDLevelEditorModule::RegisterMenus));

    // Tab注册
    // TabManager
    FGlobalTabmanager::Get()->RegisterTabSpawner(LevelCreatorTab,
        FOnSpawnTab::CreateRaw(this, &FTDLevelEditorModule::OnSpawnLevelCreatorTab))
    .SetDisplayName(LOCTEXT("LevelCreatorEditor","Level Creator Editor"))
    .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FTDLevelEditorModule::ShutdownModule()
{
    FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").UnregisterSettings("Project", "Game", "TDGlobalSettings");

    FGlobalTabmanager::Get()->UnregisterTabSpawner(LevelCreatorTab);
    
}

void FTDLevelEditorModule::RegisterMenus()
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

void FTDLevelEditorModule::OnLevelCreatorButtonClicked()
{
    FGlobalTabmanager::Get()->TryInvokeTab(LevelCreatorTab);
}

TSharedRef<SDockTab> FTDLevelEditorModule::OnSpawnLevelCreatorTab(const FSpawnTabArgs& SpawnTabArgs)
{
    return SNew(SDockTab)
    .Label(FText::FromString("Level Creator"))
    [
        SNew(SLevelCreatorPanel)
    ];
    
}


#undef LOCTEXT_NAMESPACE
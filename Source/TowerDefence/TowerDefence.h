// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FMyGameModule:public  FDefaultGameModuleImpl
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;


protected:
    void RegisterMenus();
    void OnLevelCreatorButtonClicked();
    TSharedRef<class SDockTab> OnSpawnLevelCreatorTab(const FSpawnTabArgs& SpawnTabArgs);

private:
    TSharedPtr<class FUICommandList> PluginCommands;
    static FName LevelCreatorTab;
};
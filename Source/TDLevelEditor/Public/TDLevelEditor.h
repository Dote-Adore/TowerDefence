#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Framework/Docking/TabManager.h"

class FTDLevelEditorModule:public IModuleInterface
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
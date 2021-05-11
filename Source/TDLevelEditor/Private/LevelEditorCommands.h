#pragma once
#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FLevelEditorCommands:public TCommands<FLevelEditorCommands>
{
public:
	FLevelEditorCommands()
		:TCommands<FLevelEditorCommands>(TEXT("LevelCreator"),
			NSLOCTEXT("contexts", "LevelCreator", "LevelCreator Plugin"), NAME_None,  FEditorStyle::GetStyleSetName())
	{
		
	}
	virtual void RegisterCommands() override;
public:
	TSharedPtr<FUICommandInfo> LevelCreatorAction;
};

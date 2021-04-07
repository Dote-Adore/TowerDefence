#include "LevelEditorCommands.h"

#define LOCTEXT_NAMESPACE "FLevelCreator"

void FLevelEditorCommands::RegisterCommands()
{
	UI_COMMAND(LevelCreatorAction, "LevelCreator", "Execute Level Creator Action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

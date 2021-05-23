#include "LevelTaskSystem.h"

void ULevelTaskSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
}

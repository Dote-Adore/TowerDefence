#include "PackageSystem.h"

void UPackageSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
}

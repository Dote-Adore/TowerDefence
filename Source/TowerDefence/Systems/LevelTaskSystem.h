#pragma once

#include "LevelTaskSystem.generated.h"


class UArchiveSystem;
UCLASS()
class TOWERDEFENCE_API ULevelTaskSystem:public UGameInstanceSubsystem
{
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UPROPERTY(Transient)
	UArchiveSystem* ArchiveSystem;


	void LoadTaskConfig();
};

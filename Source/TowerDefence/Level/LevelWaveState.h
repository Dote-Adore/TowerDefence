#pragma once
#include "TowerDefence/Components/StateMachineComponent.h"

struct FEnemyGenerationInfo;
class UEntityCreator;
class ABaseTile;
class UBaseWaveState:public UState
{
public:
	UBaseWaveState(UStateMachineComponent* InStateMachineCompoent, class ALevelManager* LevelManager)
        :UState(InStateMachineCompoent), OwnerLevelManager(LevelManager), CurrentWaveInfo(nullptr)
	{
	}
	virtual void OnEnter() override;
protected:
	class ALevelManager* OwnerLevelManager;
	TSharedPtr<const FEnemyGenerationInfo> CurrentWaveInfo;
};

class UInitWaveState:public UBaseWaveState
{
public:
	UInitWaveState(UStateMachineComponent* InStateMacineCompoent, class ALevelManager* LevelManager)
		: UBaseWaveState(InStateMacineCompoent, LevelManager), TotalPathNum(0)
	{
	}

	virtual void Tick(float DeltaTime) override;
	virtual void OnEnter() override;
	virtual void OnExit() override ;
	virtual FName GetStateName() override;
private:
	int32 TotalPathNum;
	int32 PlayDownPathNum = 0;
	const float GrapShowPathTime = 0.1f;
	const float EachTileDurationTime = 0.4f;
	int32 CurrentShowPathTime = 0;
	TMap<int32, float> TilesPlayAnimLeftTime;
	TSet<int32> FinishedAnimTilesIdx;
	FLinearColor TargetShowPathLinearColor = FLinearColor(0.5,1,1, 0.8);
	FLinearColor EndColor = FLinearColor(0.5,1,1, 0);
};

class UGenerateEnemiesState:public UBaseWaveState
{
public:
	UGenerateEnemiesState(UStateMachineComponent* InStateMachineComponent, class ALevelManager* LevelManager);
	virtual void Tick(float DeltaTime) override;
	virtual void OnEnter() override;
	virtual void OnExit() override ;
	virtual FName GetStateName() override;
private:
	float OnNextSpawnEnemtyTime = 0.f;
	int32 currentSpawnedIdx = 0;
	TArray<const ABaseTile*> PathTiles;
};
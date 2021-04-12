#include "LevelWaveState.h"
#include "LevelManager.h"
#include "TowerDefence/Entities/Base/Tile.h"
#include "TowerDefence/Entities/Enemy.h"
#include "LevelInfomation.h"
#include "TowerDefence/Creator/EntityCreator.h"
#include "TowerDefence/Components/EnemyMovementComponent.h"

void UBaseWaveState::OnEnter()
{
	CurrentWaveInfo = OwnerLevelManager->GetCurrentWaveInfoPtr();
}

// -------Init------------
void UInitWaveState::Tick(float DeltaTime)
{
	if(FinishedAnimTilesIdx.Num() == TotalPathNum)
	{
		OwnerStateMachine->ChangeState("GenerateEnemies");
		return;
	}
	for(auto& Pair: TilesPlayAnimLeftTime)
	{
		ABaseTile* TargetChangedTile = OwnerLevelManager->GetAllTiles()[Pair.Key];
		if(Pair.Value <= 0)
		{
			TargetChangedTile->ChangePlaneColor(EndColor);
			FinishedAnimTilesIdx.Add(Pair.Key);
			continue;
		}
		if(Pair.Value <= EachTileDurationTime)
		{
			TargetChangedTile->ChangePlaneColor
			(FMath::Lerp(EndColor, TargetShowPathLinearColor, Pair.Value/EachTileDurationTime));
		}
		Pair.Value -= DeltaTime;
	}
}

void UInitWaveState::OnEnter()
{
	UBaseWaveState::OnEnter();
	TotalPathNum = CurrentWaveInfo->Path.Num();
	for(int32 i = 0;i < TotalPathNum;i++)
	{
		TilesPlayAnimLeftTime.Add(CurrentWaveInfo->Path[i], EachTileDurationTime + GrapShowPathTime*i);
	}
}

void UInitWaveState::OnExit()
{
}


FName UInitWaveState::GetStateName()
{
	return FName("Init");
}


// ---------------------------



// -----------Generate Enemies --------

UGenerateEnemiesState::UGenerateEnemiesState(UStateMachineComponent* InStateMachineComponent,
	ALevelManager* LevelManager)
	: UBaseWaveState(InStateMachineComponent, LevelManager)
{
	EntityCreator = NewObject<UEntityCreator>(LevelManager->GetWorld());
}

void UGenerateEnemiesState::Tick(float DeltaTime)
{
	if(currentSpawnedIdx >= CurrentWaveInfo->GeneratedID.Num())
	{
		OwnerStateMachine->ChangeState("WaitForNext");
		return;
	}
	if(OnNextSpawnEnemtyTime>0)
	{
		OnNextSpawnEnemtyTime -=DeltaTime;
		return;
	}
	const ABaseTile* StartTile = PathTiles[0];
	AEnemy* TargetEnemy = EntityCreator->CreateEnemy(CurrentWaveInfo->GeneratedID[currentSpawnedIdx],
		FTransform(StartTile->GetSpawnEntityLocation()));
	UE_LOG(LogTemp, Display, TEXT("GenerateEnemiesState:Spawn Enemy Success!"))

	TargetEnemy->GetEnemyMovementComp()->SetPath(PathTiles);
	
	OnNextSpawnEnemtyTime = CurrentWaveInfo->GrapTime;
	currentSpawnedIdx ++;
}

void UGenerateEnemiesState::OnEnter()
{
	UBaseWaveState::OnEnter();
	OnNextSpawnEnemtyTime = 0;
	currentSpawnedIdx = 0;
	for(int32 PathID: CurrentWaveInfo->Path)
	{
		PathTiles.Add(OwnerLevelManager->GetAllTiles()[PathID]);
	}
}

void UGenerateEnemiesState::OnExit()
{
}

FName UGenerateEnemiesState::GetStateName()
{
	return "GenerateEnemies";
}
// -------------------------------------
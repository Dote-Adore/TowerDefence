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
	TilesPlayAnimLeftTime.Empty();
	TotalPathNum = CurrentWaveInfo->Path.Num();
	UE_LOG(LogTemp, Display, TEXT("Start To A Round %d, %d enemies Coming in......"),
		OwnerLevelManager->CurrentWaveIdx, CurrentWaveInfo->GeneratedID.Num());
	for(int32 i = 0;i < TotalPathNum;i++)
	{
		TilesPlayAnimLeftTime.Add(CurrentWaveInfo->Path[i], EachTileDurationTime + GrapShowPathTime*i);
	}
	FinishedAnimTilesIdx.Empty();
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
	const ABaseTile* NextTile = PathTiles[1];

	// 计算初始的方向

	FVector InitDirection = (NextTile->GetActorLocation() - StartTile->GetActorLocation()).GetSafeNormal2D();
	
	AEnemy* TargetEnemy = OwnerLevelManager->EntityCreator->CreateEnemy(CurrentWaveInfo->GeneratedID[currentSpawnedIdx],
		FTransform(InitDirection.ToOrientationQuat(), StartTile->GetSpawnEntityLocation()));
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
	// 这里将其初始化为0
	OwnerLevelManager->CurrentWaveEnemyDeathNum = 0;
}

void UGenerateEnemiesState::OnExit()
{
}

FName UGenerateEnemiesState::GetStateName()
{
	return "GenerateEnemies";
}

UWaitForNextState::UWaitForNextState(UStateMachineComponent* InStateMachineComponent, ALevelManager* LevelManager)
	:UBaseWaveState(InStateMachineComponent, LevelManager)
{
}

void UWaitForNextState::Tick(float DeltaTime)
{
	// 如果还没有全部死亡的话则继续等待
	if(OwnerLevelManager->CurrentWaveEnemyDeathNum<CurrentWaveInfo->GeneratedID.Num())
	{
		return;
	}
	if(OwnerLevelManager->CurrentWaveIdx<OwnerLevelManager->TotalWaves - 1)
	{
		if(DelayForNext>0)
		{
			DelayForNext -= DeltaTime;
			return;
		}
		OwnerLevelManager->OnNextWave();
		OwnerStateMachine->ChangeState("Init");
	}
	else
	{
		OwnerLevelManager->OnLevelSuccessDelegate.Broadcast();
		OwnerStateMachine->ChangeState("End");
	}
}

void UWaitForNextState::OnEnter()
{
	UBaseWaveState::OnEnter();
	DelayForNext = 2.f;
}

void UWaitForNextState::OnExit()
{
}

FName UWaitForNextState::GetStateName()
{
	return "WaitForNext";
}

// -------------------------------------

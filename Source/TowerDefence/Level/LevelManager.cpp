#include "LevelManager.h"
#include "LevelInfomation.h"
#include "TowerDefence/TDGameInstance.h"
#include "TowerDefence/Entities/Base/Tile.h"
#include "TowerDefence/Components/StateMachineComponent.h"
#include "LevelWaveState.h"
#include "TowerDefence/Entities/Turrent.h"
#include "TowerDefence/Components/EnemyMovementComponent.h"
#include "TowerDefence/Creator/EntityCreator.h"
#include "TowerDefence/Entities/Enemy.h"
#include "TowerDefence/Systems/LevelTaskSystem.h"

ALevelManager::ALevelManager(const FObjectInitializer& ObjectInitializer)
	:AActor(ObjectInitializer)
{
	// PrimaryActorTick.bCanEverTick = true;
	// PrimaryActorTick.bStartWithTickEnabled = true;
	
	
	StateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>(TEXT("WaveStateComp"));
}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	UEnemyMovementComponent::OnEnemyArrivalToEndDelegate.AddUObject(this, &ALevelManager::OnEnemyArrivalToTheEndListener);
	ATDPlayerController::OnTileSelectedEvent.AddUObject(this, &ALevelManager::OnTileClickedListener);

	EntityCreator = NewObject<UEntityCreator>(this);
		UTDGameInstance* TDGameInstance = Cast<UTDGameInstance>(GetGameInstance());
	check(TDGameInstance);
	
	AEnemy::OnEnemyDeathEvent.AddUObject(this, &ALevelManager::OnEnemyDeathListener);
	ULevelTaskSystem* LevelTaskSys = TDGameInstance->GetSubsystem<ULevelTaskSystem>();
	CurrentLevelTaskItem = LevelTaskSys->GetCurrentLevelTaskItem();
	TotalWaves = CurrentLevelTaskItem.ConfigData.TargetLevelInfo->Waves.Num();
	// 初始化部署点数
	DeployPoint = CurrentLevelTaskItem.ConfigData.TargetLevelInfo->InitDeployPoints;
	SurplusEnemyArrivalNum = CurrentLevelTaskItem.ConfigData.TargetLevelInfo->MaxEnemyToEndNums;
	CurrentWaveIdx = -1;

	GenerateLevelMap();

	
	StateMachineComponent->RegisterState(
		MakeShared<UInitWaveState>(UInitWaveState(StateMachineComponent, this)));
	StateMachineComponent->RegisterState(
		MakeShared<UGenerateEnemiesState>(UGenerateEnemiesState(StateMachineComponent, this)));
	StateMachineComponent->RegisterState(
		MakeShared<UWaitForNextState>(UWaitForNextState(StateMachineComponent, this)));
	GetWorldTimerManager().SetTimer(StartGameDelayTimeHandle, [&]()->void
	{
		GetWorldTimerManager().ClearTimer(StartGameDelayTimeHandle);
		OnNextWave();
		StateMachineComponent->ChangeState("init");
	}, 2.f, false);

}

void ALevelManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALevelManager::GenerateLevelMap()
{
	const FTileInfo& Tiles = CurrentLevelTaskItem.ConfigData.TargetLevelInfo->TileInfo;
	for(int i = 0; i < Tiles.Height; i++)
	{
		for(int j = 0; j < Tiles.Witdh; j++)
		{
			UClass* TargetGeneratedClass = Tiles.Tiles[i*Tiles.Witdh+j];
			int32 YOffset =(-Tiles.Height+1+2*i) * ABaseTile::BoxSize/2;
			int32 XOffset = (-Tiles.Witdh+1+2*j) * ABaseTile::BoxSize/2;
			ABaseTile* TargetTile = GetWorld()->SpawnActor<ABaseTile>(TargetGeneratedClass);
			TargetTile->SetActorLocation(FVector(XOffset*2, YOffset*2, 0));
			AllTiles.Add(TargetTile);
		}
	}
}

void ALevelManager::OnTileClickedListener(ABaseTile* TargetTile)
{
	if(TargetGeneratedID == INDEX_NONE||TargetTile->CanDeploy(TargetGeneratedCategory) == false)
	{
		OnCancelDeploy();
		OnDeployTurrentEvent.Broadcast(nullptr, nullptr);
		return;
	}
	ATurrent* SpawnedTurrent = EntityCreator->CreateTurrent(TargetGeneratedID,
        FTransform(TargetTile->GetSpawnEntityLocation()));
	check(SpawnedTurrent);
	TargetTile->SetDeployEntity(SpawnedTurrent);
	DeployPoint -= TargetDeployCost;
	OnDeployTurrentEvent.Broadcast(SpawnedTurrent, TargetTile);
	OnCancelDeploy();
}

void ALevelManager::OnEnemyDeathListener(AEnemy* TargetDeathEnemy)
{	
    DeployPoint += TargetDeathEnemy->GetCurrentEntityParams().DeployPoints;
	CurrentWaveEnemyDeathNum++;
}

void ALevelManager::OnEnemyArrivalToTheEndListener(AEnemy* TargetDeathEnemy)
{
	SurplusEnemyArrivalNum --;
	TargetDeathEnemy->Destroy();
	// 如果到达终点的敌人数量超过通关限定条件，则说明游戏失败,这里只执行一次
	if(SurplusEnemyArrivalNum == -1)
	{
		// 游戏失败
		OnLevelFailedDelegate.Broadcast();
		GetWorld()->GetFirstPlayerController()->SetPause(true);
	}
	
}

TSharedPtr<const FEnemyGenerationInfo> ALevelManager::GetCurrentWaveInfoPtr()
{
    const FEnemyGenerationInfo& TargetWave = CurrentLevelTaskItem.ConfigData.TargetLevelInfo->Waves[CurrentWaveIdx];
    return MakeShared<FEnemyGenerationInfo>(TargetWave);
}

void ALevelManager::OnLevelSuccess()
{
}

void ALevelManager::OnRequestToDeploy(int32 TurrentID, FName Category, int32 Cost)
{
	OnCancelDeploy();
	TArray<ABaseTile*> Res;
	for(auto Tile:AllTiles)
	{
		if(Tile->CanDeploy(Category))
		{
			Res.Add(Tile);
		}	
	}
	for(auto Tile:Res)
	{
		Tile->ChangePlaneColor(CanDeployColor);
	}
	TargetGeneratedID = TurrentID;
	TargetGeneratedCategory = Category;
	TargetDeployCost = Cost;
}


void ALevelManager::OnCancelDeploy()
{
	TargetGeneratedCategory = NAME_None;
	TargetGeneratedID = INDEX_NONE;
	for(auto Tile:AllTiles)
	{
		Tile->ChangePlaneColor(FLinearColor(0,0,0,0));
	}
}

void ALevelManager::OnNextWave()
{
	CurrentWaveIdx++;
	OnNextWaveDeleagate.Broadcast();
}

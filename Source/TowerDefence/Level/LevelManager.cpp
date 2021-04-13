#include "LevelManager.h"
#include "LevelInfomation.h"
#include "TowerDefence/TDGameInstance.h"
#include "TowerDefence/Entities/Base/Tile.h"
#include "TowerDefence/Components/StateMachineComponent.h"
#include "LevelWaveState.h"
#include "TowerDefence/Entities/Turrent.h"
#include "TowerDefence/Creator/EntityCreator.h"

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
	TDGameInstance = Cast<UTDGameInstance>(GetGameInstance());
	check(TDGameInstance);
	GenerateLevelMap();
	EntityCreator = NewObject<UEntityCreator>(GetWorld());

	
	TotalWaves = TDGameInstance->CurrentLevelInfomation->Waves.Num();
	// 初始化部署点数
	DeployPoint = TDGameInstance->CurrentLevelInfomation->InitDeployPoints;

	
	CurrentWaveIdx = 0;
	RoundState = Init;
	StateMachineComponent->RegisterState(
		MakeShared<UInitWaveState>(UInitWaveState(StateMachineComponent, this)));
	StateMachineComponent->RegisterState(
		MakeShared<UGenerateEnemiesState>(UGenerateEnemiesState(StateMachineComponent, this)));
	StateMachineComponent->ChangeState("init");
}

void ALevelManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALevelManager::GenerateLevelMap()
{
	const FTileInfo& Tiles = TDGameInstance->CurrentLevelInfomation->TileInfo;
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

TSharedPtr<const FEnemyGenerationInfo> ALevelManager::GetCurrentWaveInfoPtr()
{
    const FEnemyGenerationInfo& TargetWave = TDGameInstance->CurrentLevelInfomation->Waves[CurrentWaveIdx];
    return MakeShared<FEnemyGenerationInfo>(TargetWave);
}

TArray<ABaseTile*> ALevelManager::OnGetDeployableTiles(int32 TurrentID, FName Category)
{
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
	return Res;
}

void ALevelManager::RequsetDeployToTile(int32 TurrentID, ABaseTile* TargetTile, int32 Cost)
{
	AEntity* SpawnedTurrent = EntityCreator->CreateTurrent(TurrentID,
		FTransform(TargetTile->GetSpawnEntityLocation()));
	check(SpawnedTurrent);
	TargetTile->SetDeployEntity(SpawnedTurrent);
	// 将所有tile的颜色去除
	OnCancelDeploy();
	DeployPoint -= Cost;
}

void ALevelManager::OnCancelDeploy()
{
	for(auto Tile:AllTiles)
	{
		Tile->ChangePlaneColor(FLinearColor(0,0,0,0));
	}
}

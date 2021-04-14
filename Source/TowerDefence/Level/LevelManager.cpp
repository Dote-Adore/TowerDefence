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
	ATDPlayerController::OnTileSelectedEvent.AddUObject(this, &ALevelManager::OnTileClickedListener);
}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	EntityCreator = NewObject<UEntityCreator>(this);
	TDGameInstance = Cast<UTDGameInstance>(GetGameInstance());
	check(TDGameInstance);
	GenerateLevelMap();

	
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

void ALevelManager::OnTileClickedListener(ABaseTile* TargetTile)
{
	if(TargetGenereatedID == INDEX_NONE||TargetTile->CanDeploy(TargetGeneratedCategory) == false)
	{
		return;
	}
	AEntity* SpawnedTurrent = EntityCreator->CreateTurrent(TargetGenereatedID,
        FTransform(TargetTile->GetSpawnEntityLocation()));
	check(SpawnedTurrent);
	TargetTile->SetDeployEntity(SpawnedTurrent);
	DeployPoint -= TargetDeployCost;
	OnCancelDeploy();
}

TSharedPtr<const FEnemyGenerationInfo> ALevelManager::GetCurrentWaveInfoPtr()
{
    const FEnemyGenerationInfo& TargetWave = TDGameInstance->CurrentLevelInfomation->Waves[CurrentWaveIdx];
    return MakeShared<FEnemyGenerationInfo>(TargetWave);
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
	TargetGenereatedID = TurrentID;
	TargetGeneratedCategory = Category;
	TargetDeployCost = Cost;
}


void ALevelManager::OnCancelDeploy()
{
	TargetGeneratedCategory = NAME_None;
	TargetGenereatedID = INDEX_NONE;
	for(auto Tile:AllTiles)
	{
		Tile->ChangePlaneColor(FLinearColor(0,0,0,0));
	}
}

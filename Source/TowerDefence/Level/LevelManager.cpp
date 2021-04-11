#include "LevelManager.h"
#include "LevelInfomation.h"
#include "TowerDefence/TDGameInstance.h"
#include "TowerDefence/Entities/Base/Tile.h"

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	TDGameInstance = Cast<UTDGameInstance>(GetGameInstance());
	check(TDGameInstance);
	GenerateLevelMap();
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
			AActor* TargetTile = GetWorld()->SpawnActor(TargetGeneratedClass);
			TargetTile->SetActorLocation(FVector(XOffset*2, YOffset*2, 0));
		}
	}
}

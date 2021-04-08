#pragma once
#include <basetyps.h>
#include "LevelInfomation.generated.h"

USTRUCT(BlueprintType)
struct FTileInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int32 Height;
	UPROPERTY(EditAnywhere)
	int32 Witdh;
	// 所有的地块保存到这里
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ABaseTile>> Tiles;
};

// 每一波怪物生成
USTRUCT(BlueprintType)
struct FEnemyGenerationInfo
{
	GENERATED_BODY()
	// 每生成一个怪物的间隔时间
	UPROPERTY(EditAnywhere)
	float GrapTime;
	// 生成的怪物ID
	UPROPERTY(EditAnywhere)
	TArray<int32> GeneratedID;
	// 配置的路径
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> Path;
};


// 每一个关卡的Level信息，包括地图数据，每一波的怪物配置，使用的怪物ID等等
UCLASS(BlueprintType)
class ULevelInfomation:public UDataAsset
{
	GENERATED_BODY()
public:
	// 地图信息
	UPROPERTY(EditAnywhere)
	FTileInfo TileInfo;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UWorld> BackgroundWorld;
	// 生成怪物波数设定
	UPROPERTY(EditAnywhere)
	TArray<FEnemyGenerationInfo> Waves;
	
};

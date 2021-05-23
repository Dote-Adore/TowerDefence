
#pragma once
#include "Engine/DataTable.h"
// #include "TowerDefence/Level/LevelInfomation.h"

#include "LevelTaskData.generated.h"

/*
 *关卡任务系统的数据，包括在选择关卡地图的时候的位置
 *名称、解锁前置条件等
 */

// 解锁关卡的条件
USTRUCT()
struct FUnlockRequirment
{
	GENERATED_BODY()
	// 解锁该关卡的条件是这里需要完成的关卡id
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<int32> FinishedLevels;
	// 总共获得的星星数
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TSet<int32> TotalStarNum;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
};

// 通关完成后的奖励
USTRUCT()
struct FFinishReward
{
	GENERATED_BODY()
	// 角色培养素材, id对数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, int32> DevelopItems;
	// 食物,id对数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32,int32> Foods;
	// 付费金币奖励
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PaidCoin;
	// 游戏金币奖励
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GameCoin;
};


USTRUCT(BlueprintType)
struct FLevelTaskData:public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;
	// 在地图的位置
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2D TranslationInMap;
	// 目标使用的LevelInfomation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULevelInfomation* TargetLevelInfo;
	// 关卡的介绍
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelIntro;
	// 解锁的条件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUnlockRequirment UnlockRequirment;
	// 第一次通关的奖励
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFinishReward FirstFinishRewards;
	// 正常每一次游玩会提供的奖励
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFinishReward NormalRewards;
};
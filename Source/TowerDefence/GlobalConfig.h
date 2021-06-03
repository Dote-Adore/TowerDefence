﻿#pragma once
#include "GlobalConfig.generated.h"


class ATurrent;
class AEnemy;
class UDataTable;
struct FBuff;
UCLASS(Config=Game)
class TOWERDEFENCE_API UGlobalConfig:public UObject
{
    typedef TMap<FName, class ULevelInfomation*> AllLevelsMap;
    GENERATED_BODY()
public:
    UPROPERTY(Config, EditAnywhere)
    TSoftObjectPtr<UDataTable> TurrentDataTable;
    UPROPERTY(Config, EditAnywhere)
    TSoftObjectPtr<UDataTable> EnemyDataTable;
    UPROPERTY(Config, EditAnywhere)
    TSoftObjectPtr<UDataTable> TurrentAnimDataTable;
    UPROPERTY(Config, EditAnywhere)
    TSoftObjectPtr<UDataTable> EnemyAnimDataTable;
    
    // buff 配置表
    UPROPERTY(Config, EditAnywhere)
    TSoftObjectPtr<UDataTable> BuffDataTable;

    
    UPROPERTY(Config,EditAnywhere)
    TSubclassOf<ATurrent> TurrentEntityClass;
    UPROPERTY(Config,EditAnywhere)
    TSubclassOf<AEnemy> EnemyEntityClass;


    // 炮塔额外的表格
    UPROPERTY(Config, EditAnywhere)
    TSoftObjectPtr<UDataTable> TurrentAdditialTable;
    
    // 所有的Tile品类
    UPROPERTY(Config, EditAnywhere, Category=LevelEditor)
    TMap<FName, TSubclassOf<class ABaseTile>> AllTiles;
    // 整个游戏所有的关卡存放的位置
    UPROPERTY(Config, EditAnywhere, Category=LevelEditor)
    FString TargetLevelsPath;
    UPROPERTY(Config, EditAnywhere, Category=LevelTask)
    TSoftObjectPtr<UDataTable> LevelTaskDataTable;
    // 角色升级策略配置表
    UPROPERTY(Config, EditAnywhere, Category=CharacterSystem)
    TSoftObjectPtr<UDataTable> CharacterRankConfigTable;
    // 角色升级需要用到的id和对应的每个拥有的点数
    UPROPERTY(Config, EditAnywhere, Category=CharacterSystem)
    TMap<int32, int32> CharacterUpgradeDevelopItemIDAndPointMap;
    
    // 角色升级Item
    UPROPERTY(Config, EditAnywhere, Category=PackageSystem)
    TSoftObjectPtr<UDataTable> DevelopmentItems;
    // 武器Item
    UPROPERTY(Config, EditAnywhere, Category=PackageSystem)
    TSoftObjectPtr<UDataTable> WeaponItems;
    // 食物 Item
    UPROPERTY(Config, EditAnywhere, Category=PackageSystem)
    TSoftObjectPtr<UDataTable> FoodItems;
    
    UPROPERTY(Config,EditAnywhere,Category=UserSystem)
    // 体力恢复时间
    int32 PhysicalStrengthRecoverySeconds = 60;
    // 最大体力
    UPROPERTY(Config,EditAnywhere,Category=UserSystem)
    int32 MaxPhysicalStrength = 120;
    const FBuff* FindBuffByID(int32 ID);
    const AllLevelsMap GetAllLevels() const;
private:
    TMap<int32, FBuff*> LoadedBuffs;
    // 所有的Levelsload进来
    mutable AllLevelsMap LoadedLevels;
};
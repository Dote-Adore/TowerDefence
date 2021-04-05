#pragma once
#include "Entities/Base/Entity.h"
#include "GlobalConfig.generated.h"



struct FBuff;
UCLASS(Config=Game)
class UGlobalConfig:public UObject
{
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
    TSubclassOf<UAnimInstance> TurrentAnimInstClass;
    UPROPERTY(Config,EditAnywhere)
    TSubclassOf<UAnimInstance> EnemyAnimInstClass;

    const FBuff* FindBuffByID(int32 ID);
private:
    TMap<int32, FBuff*> LoadedBuffs;
    
};
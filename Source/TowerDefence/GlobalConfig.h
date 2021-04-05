#pragma once
#include "Entities/Base/Entity.h"
#include "GlobalConfig.generated.h"


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
    UPROPERTY(Config,EditAnywhere)
    TSubclassOf<UAnimInstance> TurrentAnimInstClass;
    UPROPERTY(Config, EditAnywhere)
    TSubclassOf<UAnimComponent> TurrentAnimComp;
    UPROPERTY(Config, EditAnywhere)
    TSubclassOf<UAnimComponent> EnemyAnimComp;
    UPROPERTY(Config,EditAnywhere)
    TSubclassOf<UAnimInstance> EnemyAnimInstClass;
};
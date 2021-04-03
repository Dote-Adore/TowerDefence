#include "EntityCreator.h"

#include "TowerDefence/GlobalConfig.h"
#include "TowerDefence/Entities/Turrent.h"

void UEntityCreator::CreateTurrent(int32 EntityID, FTransform EntityTransform)
{
    const UGlobalConfig* GlobalConfig = GetDefault<UGlobalConfig>();
    UDataTable* TurrentDataTable = GlobalConfig->TurrentDataTable.LoadSynchronous();
    UDataTable* AnimDataTable = GlobalConfig->TurrentAnimDataTable.LoadSynchronous();
    check(TurrentDataTable);
    check(AnimDataTable);
    CreateEntity(EntityID, EntityTransform, ATurrent::StaticClass(), TurrentDataTable, AnimDataTable);
}

void UEntityCreator::CreateEnemy(int32 EntityID, FTransform EntityTransform)
{
    const UGlobalConfig* GlobalConfig = GetDefault<UGlobalConfig>();
    UDataTable* EnemyDataTable = GlobalConfig->EnemyDataTable.LoadSynchronous();
    UDataTable* AnimDataTable = GlobalConfig->EnemyAnimDataTable.LoadSynchronous();
    check(EnemyDataTable);
    check(AnimDataTable);
    CreateEntity(EntityID, EntityTransform, ATurrent::StaticClass(), EnemyDataTable, AnimDataTable);
}

void UEntityCreator::CreateEntity(int32 EntityID, FTransform EntityTransform, TSubclassOf<AEntity> Entitylass, UDataTable* EntityDatas, UDataTable* AnimDatas)
{
    const UGlobalConfig* GlobalConfig = GetDefault<UGlobalConfig>();
    FEntityParams* TargetFoundParams =  EntityDatas->FindRow<FEntityParams>(GET_MEMBER_NAME_CHECKED(FEntityParams, EntityID), FString::FromInt(EntityID));
    FEntityAnimation* TargetFoundAnims = AnimDatas->FindRow<FEntityAnimation>(GET_MEMBER_NAME_CHECKED(FEntityAnimation, EntityID), FString::FromInt(EntityID));
    check(TargetFoundParams);
    check(TargetFoundAnims);
    AEntity* SpawnedEntity = GetWorld()->SpawnActor<AEntity>(Entitylass, EntityTransform);
    SpawnedEntity->InitEntity(*TargetFoundParams, *TargetFoundAnims, EntityTransform, {});
}


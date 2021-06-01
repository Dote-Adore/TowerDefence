﻿#include "EntityCreator.h"


#include "Components/CapsuleComponent.h"
#include "TowerDefence/GlobalConfig.h"
#include "TowerDefence/Entities/Turrent.h"
#include "TowerDefence/Entities/Enemy.h"
#include "TowerDefence/Systems/CharacterSystem.h"

ATurrent* UEntityCreator::CreateTurrent(int32 EntityID, FTransform EntityTransform)
{
    const UGlobalConfig* GlobalConfig = GetDefault<UGlobalConfig>();
    UDataTable* TurrentDataTable = GlobalConfig->TurrentDataTable.LoadSynchronous();
    UDataTable* AnimDataTable = GlobalConfig->TurrentAnimDataTable.LoadSynchronous();
    check(TurrentDataTable);
    check(AnimDataTable);
    AEntity* Res = CreateEntity(EntityID, EntityTransform, GlobalConfig->TurrentEntityClass, TurrentDataTable, AnimDataTable);
    FCharacterSavedInfo SavedInfo = GetWorld()->GetGameInstance()->GetSubsystem<UCharacterSystem>()->GetCharacterSavedInfo(EntityID);
    // 根据等级修改参数
    Res->GetCurrentEntityParams().Attack = SavedInfo.Attack;
    Res->GetCurrentEntityParams().Attack = SavedInfo.Defence;
    Res->GetCurrentEntityParams().Attack = SavedInfo.MaxHP;
    return Cast<ATurrent>(Res);
}

AEnemy* UEntityCreator::CreateEnemy(int32 EntityID, FTransform EntityTransform)
{
    const UGlobalConfig* GlobalConfig = GetDefault<UGlobalConfig>();
    UDataTable* EnemyDataTable = GlobalConfig->EnemyDataTable.LoadSynchronous();
    UDataTable* AnimDataTable = GlobalConfig->EnemyAnimDataTable.LoadSynchronous();
    check(EnemyDataTable);
    check(AnimDataTable);
    AEntity* Res = CreateEntity(EntityID, EntityTransform, GlobalConfig->EnemyEntityClass, EnemyDataTable, AnimDataTable);
    return Cast<AEnemy>(Res);
}

AEntity* UEntityCreator::CreateEntity(int32 EntityID, FTransform EntityTransform, TSubclassOf<AEntity> Entitylass, UDataTable* EntityDatas, UDataTable* AnimDatas)
{
    FEntityParams* TargetFoundParams =  EntityDatas->FindRow<FEntityParams>(FName(*FString::FromInt(EntityID)), "EntityID");
    FEntityAnimation* TargetFoundAnims = AnimDatas->FindRow<FEntityAnimation>(FName(*FString::FromInt(EntityID)), "EntityID");
    check(TargetFoundParams);
    check(TargetFoundAnims);
    
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    check(GetWorld());
    AEntity* SpawnedEntity = GetWorld()->SpawnActor<AEntity>(Entitylass, EntityTransform, SpawnParameters);
    EntityTransform.SetLocation(EntityTransform.GetLocation()+FVector(0,0,
        SpawnedEntity->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
    SpawnedEntity->InitEntity(*TargetFoundParams, *TargetFoundAnims, EntityTransform, {});
    return SpawnedEntity;
}


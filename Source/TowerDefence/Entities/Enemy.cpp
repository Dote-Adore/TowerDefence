#include "Enemy.h"

#include "Turrent.h"
#include "TowerDefence/GlobalConfig.h"
#include "TowerDefence/Components/EnemyAnimComponent.h"

AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer):
AEntity(ObjectInitializer)
{
    TargetAttackEntityClass = ATurrent::StaticClass();
    const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
    AnimInstanceClass = Config->EnemyAnimInstClass;
}

TSubclassOf<UAnimComponent> AEnemy::GetAnimCompClass() const
{
    return UEnemyAnimComponent::StaticClass();
}

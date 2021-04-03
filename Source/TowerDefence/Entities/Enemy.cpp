#include "Enemy.h"

#include "Turrent.h"
#include "TowerDefence/GlobalConfig.h"

AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer):
AEntity(ObjectInitializer)
{
    TargetAttackEntityClass = ATurrent::StaticClass();
    const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
    AnimInstanceClass = Config->EnemyAnimInstClass;
}

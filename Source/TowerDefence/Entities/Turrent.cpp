#include "Turrent.h"
#include "Enemy.h"
#include "TowerDefence/GlobalConfig.h"

class UGlobalConfig;

ATurrent::ATurrent(const FObjectInitializer& ObjectInitializer)
    :AEntity(ObjectInitializer)
{
    TargetAttackEntityClass = AEnemy::StaticClass();
    
}

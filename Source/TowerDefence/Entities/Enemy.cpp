#include "Enemy.h"

#include "Turrent.h"
#include "TowerDefence/GlobalConfig.h"
#include "TowerDefence/Components/EnemyAnimComponent.h"
#include "TowerDefence/Components/EnemyMovementComponent.h"

FOnEnemyDeathEvent AEnemy::OnEnemyDeathEvent = FOnEnemyDeathEvent();

AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer):
AEntity(ObjectInitializer)
{
    TargetAttackEntityClass = ATurrent::StaticClass();
    const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
    AnimInstanceClass = Config->EnemyAnimInstClass;
    EnemyMovementComponent = CreateDefaultSubobject<UEnemyMovementComponent>(TEXT("EnemyMovementComp"));
}

UEnemyMovementComponent* AEnemy::GetEnemyMovementComp() const
{
    return EnemyMovementComponent;
}

TSubclassOf<UAnimComponent> AEnemy::GetAnimCompClass() const
{
    return UEnemyAnimComponent::StaticClass();
}

void AEnemy::OnDeath()
{
    Super::OnDeath();
    OnEnemyDeathEvent.Broadcast(this);
}

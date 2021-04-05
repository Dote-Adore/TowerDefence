#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TowerDefence/GlobalConfig.h"

ABullet::ABullet(const FObjectInitializer& ObjectInitializer)
    :AActor(ObjectInitializer)
{
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
    SetRootComponent(StaticMeshComponent);
}

void ABullet::File(AEntity* ParentEntity, AEntity* TargetAttackEntity, const FEntityHitAttack& CurrentAttack)
{
}

void AImmediateHitBullet::File(AEntity* ParentEntity, AEntity* TargetAttackEntity,
                               const FEntityHitAttack& CurrentAttack)
{
    // 计算攻击值
    float AttackValue = ParentEntity->GetCurrentEntityParams().Attack*CurrentAttack.AttackRate;
    UGlobalConfig* GlobalConf = GetMutableDefault<UGlobalConfig>();
    const FBuff* Buff = GlobalConf->FindBuffByID(CurrentAttack.BuffID);
    TargetAttackEntity->OnDamage(AttackValue, Buff);
    Destroy();
}

void AFilghtHitBullet::File(AEntity* ParentEntity, AEntity* TargetAttackEntity, const FEntityHitAttack& CurrentAttack)
{
}

#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
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
    // GetBuff
    FBuff Buff;
    TargetAttackEntity->OnDamage(AttackValue, Buff);
}

void AFilghtHitBullet::File(AEntity* ParentEntity, AEntity* TargetAttackEntity, const FEntityHitAttack& CurrentAttack)
{
}

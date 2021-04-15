#include "Bullet.h"

#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TowerDefence/GlobalConfig.h"
#include "Components/SplineComponent.h"

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

AFilghtHitBullet::AFilghtHitBullet(const FObjectInitializer& ObjectInitializer)
    :ABullet(ObjectInitializer)
{
    BulletFlightSplinePathComp = CreateDefaultSubobject<USplineComponent>("FlightPathSplineComp");
    BulletFlightSplinePathComp->SetHiddenInGame(false);
    BulletFlightSplinePathComp->SetUsingAbsoluteLocation(true);
}

void AFilghtHitBullet::File(AEntity* ParentEntity, AEntity* TargetAttackEntity, const FEntityHitAttack& CurrentAttack)
{
    float AttackValue = ParentEntity->GetCurrentEntityParams().Attack*CurrentAttack.AttackRate;
    UGlobalConfig* GlobalConf = GetMutableDefault<UGlobalConfig>();
    const FBuff* Buff = GlobalConf->FindBuffByID(CurrentAttack.BuffID);
    BulletFlightSplinePathComp->ClearSplinePoints();
  //   FSplinePoint StartPoint;
  //   StartPoint.Position = ParentEntity->GetActorLocation();
  // //  StartPoint.ArriveTangent = ParentEntity->GetActorLocation() - FVector(0,0,100);
  //   
  //   ButtonFlightSplinePathComp->AddPoint(StartPoint,true);
  //   FSplinePoint EndPoint;
  //   EndPoint.Type = ESplinePointType::CurveCustomTangent;
  //   EndPoint.Position = TargetAttackEntity->GetActorLocation();
    // EndPoint.LeaveTangent = TargetAttackEntity->GetActorLocation() - FVector(0,0,100);
    //ButtonFlightSplinePathComp->AddPoint(EndPoint, true);
   BulletFlightSplinePathComp->AddSplinePoint(ParentEntity->GetActorLocation(), ESplineCoordinateSpace::World);
    FVector MiddlePoint = (ParentEntity->GetActorLocation() +TargetAttackEntity->GetActorLocation())/2 +FVector(0,0,100);
    BulletFlightSplinePathComp->AddSplinePoint(MiddlePoint,ESplineCoordinateSpace::World);
   BulletFlightSplinePathComp->AddSplinePoint(TargetAttackEntity->GetActorLocation(), ESplineCoordinateSpace::World, true);
    TargetAttackEntity->OnDamage(AttackValue, Buff);
}

void AFilghtHitBullet::Tick(float DeltaSeconds)
{
    
}

#include "Bullet.h"

#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TowerDefence/GlobalConfig.h"
#include "Components/SplineComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetSystemLibrary.h"

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
    FTransform HitTransform = TargetAttackEntity->GetActorTransform();
    TargetAttackEntity->OnDamage(AttackValue, Buff, HitTransform);
    Destroy();
}

AFilghtHitBullet::AFilghtHitBullet(const FObjectInitializer& ObjectInitializer)
    :ABullet(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    BulletFlightSplinePathComp = CreateDefaultSubobject<USplineComponent>("FlightPathSplineComp");
    BulletFlightSplinePathComp->SetHiddenInGame(false);
    BulletFlightSplinePathComp->SetUsingAbsoluteLocation(true);
}

void AFilghtHitBullet::File(AEntity* ParentEntity, AEntity* TargetAttackEntity, const FEntityHitAttack& CurrentAttack)
{
    FEntityParams& Params =  ParentEntity->GetCurrentEntityParams();
    AttackValue = Params.Attack*CurrentAttack.AttackRate;
    UGlobalConfig* GlobalConf = GetMutableDefault<UGlobalConfig>();
    Buff = GlobalConf->FindBuffByID(CurrentAttack.BuffID);
    BulletFlightSplinePathComp->ClearSplinePoints();
    USkeletalMeshSocket* TargetSocket = ParentEntity->GetMesh()->SkeletalMesh->FindSocket("SpawnBullet");
    FVector MiddlePoint;
    if(TargetSocket)
    {
        BulletFlightSplinePathComp->AddSplinePoint(TargetSocket->GetSocketLocation(ParentEntity->GetMesh()), ESplineCoordinateSpace::World);
        float XYLength = FVector::Distance(TargetSocket->GetSocketLocation(ParentEntity->GetMesh()), TargetAttackEntity->GetActorLocation());
        MiddlePoint = (TargetSocket->GetSocketLocation(ParentEntity->GetMesh()) +TargetAttackEntity->GetActorLocation())/2 +FVector(0,0,XYLength/4);
    }
    else
    {
        BulletFlightSplinePathComp->AddSplinePoint(ParentEntity->GetActorLocation(), ESplineCoordinateSpace::World);
        float XYLength = FVector::Distance(ParentEntity->GetActorLocation(), TargetAttackEntity->GetActorLocation());        
        MiddlePoint = (ParentEntity->GetActorLocation() +TargetAttackEntity->GetActorLocation())/2 +FVector(0,0,XYLength/4);
    }
    MyTargetAttackEntity = TargetAttackEntity;
    BulletFlightSplinePathComp->AddSplinePoint(MiddlePoint,ESplineCoordinateSpace::World);
    BulletFlightSplinePathComp->AddSplinePoint(TargetAttackEntity->GetActorLocation(), ESplineCoordinateSpace::World, true);
    StartFlight = true;
    BulletPathLength = BulletFlightSplinePathComp->GetSplineLength();
}

void AFilghtHitBullet::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if(StartFlight == false)
    {
        return;
    }
    FVector TargetLocation = BulletFlightSplinePathComp->GetLocationAtTime(CurrentFlightTime, ESplineCoordinateSpace::World);
    FVector TargetDirection = BulletFlightSplinePathComp->GetDirectionAtTime(CurrentFlightTime,ESplineCoordinateSpace::World);
    SetActorLocation(TargetLocation);
    SetActorRotation(TargetDirection.Rotation());
    if(CurrentFlightTime >=1)
    {
        MyTargetAttackEntity->OnDamage(AttackValue, Buff, this->GetActorTransform());
        Destroy();
    }
    CurrentFlightTime += Speed*DeltaSeconds/BulletPathLength;
    
}

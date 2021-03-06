#pragma once
#include "CoreMinimal.h"
#include "Entity.h"
#include "Bullet.generated.h"


class USplineComponent;
UCLASS(BlueprintType)
class ABullet:public AActor
{
    GENERATED_BODY()
public:
    ABullet(const FObjectInitializer& ObjectInitializer);
    
    virtual void File(AEntity* ParentEntity, AEntity* TargetAttackEntity, const FEntityHitAttack& CurrentAttack) ;


protected:
    UStaticMeshComponent* StaticMeshComponent;
};

// 生成后立即生效（攻击的子弹）
UCLASS(BlueprintType)
class AImmediateHitBullet:public ABullet
{
    GENERATED_BODY()
public:
    virtual void File(AEntity* ParentEntity, AEntity* TargetAttackEntity, const FEntityHitAttack& CurrentAttack) override;
};



// 生成后需要飞行一段时间才能生效的子弹
UCLASS(BlueprintType)
class AFilghtHitBullet:public ABullet
{
    GENERATED_BODY()
public:
    AFilghtHitBullet(const FObjectInitializer& ObjectInitializer);
    // 飞行速度,每秒
    UPROPERTY(EditDefaultsOnly)
    float Speed = 300;
    virtual void File(AEntity* ParentEntity, AEntity* TargetAttackEntity, const FEntityHitAttack& CurrentAttack) override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(VisibleAnywhere)
    USplineComponent* BulletFlightSplinePathComp;

private:
    bool StartFlight = false;
    float CurrentFlightTime;
    float BulletPathLength;
    float AttackValue;
    AEntity* MyTargetAttackEntity;
    const FBuff* Buff;
};
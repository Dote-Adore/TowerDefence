#pragma once
#include "CoreMinimal.h"
#include "Entity.h"
#include "Bullet.generated.h"

UCLASS(BlueprintType)
class ABullet:public AActor
{
    GENERATED_BODY()
public:
    
    void InitBullet(AEntity* ParentEntity, AEntity* TargetAttackEntity, const FEntityHitAttack& CurrentAttack);
private:
    FBuff Buff;
    AEntity* Parent;
};

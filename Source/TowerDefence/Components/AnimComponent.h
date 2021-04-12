#pragma once
#include "Components/ActorComponent.h"
#include "AnimComponent.generated.h"


class AEntity;
UCLASS(BlueprintType)
class UAnimComponent:public UActorComponent
{
    GENERATED_BODY()
public:
    UPROPERTY()
    FName IdleAnimParamName = "Idle_WalkAnim";
    FName DeathAnimParamName = "DeathAnim";
    FName AttackAnimSlotName = "Attack";
    
    
    virtual void BeginPlay() override;
protected :
    virtual void OnInit();
    virtual void OnAttack(int32 AttackIdx);
    void OnDeath();
    AEntity* ParentEntity;
    UAnimInstance* AnimInst;
    
};

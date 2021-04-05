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
    FName IdleAnimParamName = "IdleAnim";
    FName DeathAnimParamName = "DeathAnim";
    FName AttackAnimSlotName = "Attack";
    
    
    virtual void BeginPlay() override;
private:
    void OnInit();
    void OnAttack(int32 AttackIdx);
    void OnDeath();
    AEntity* ParentEntity;
    UAnimInstance* AnimInst;
    
};

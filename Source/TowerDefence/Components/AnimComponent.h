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
    FName AttackAnimParamName;
    
    
    virtual void BeginPlay() override;
private:
    void OnInit();
    AEntity* ParentEntity;
};

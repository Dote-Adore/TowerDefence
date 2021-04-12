#pragma once
#include "Base/Entity.h"
#include "Enemy.generated.h"

class UEnemyMovementComponent;
class AEnemy;
DECLARE_EVENT_OneParam(AEnemy, FOnEnemyDeathEvent, AEnemy*)
UCLASS()
class AEnemy:public AEntity
{
    GENERATED_BODY()
public:
    AEnemy(const FObjectInitializer& ObjectInitializer);
    UEnemyMovementComponent* GetEnemyMovementComp() const;
    
    static FOnEnemyDeathEvent OnEnemyDeathEvent;
protected:
   // virtual TSubclassOf<UAnimComponent> GetAnimCompClass() const override;
    virtual void OnDeath() override;

    UEnemyMovementComponent* EnemyMovementComponent;
    
};

#pragma once
#include "Base/Entity.h"
#include "Enemy.generated.h"

UCLASS()
class AEnemy:public AEntity
{
    GENERATED_BODY()
public:
    AEnemy(const FObjectInitializer& ObjectInitializer);
protected:
    virtual TSubclassOf<UAnimComponent> GetAnimCompClass() const override;
};

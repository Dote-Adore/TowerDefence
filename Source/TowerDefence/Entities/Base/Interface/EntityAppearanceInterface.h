#pragma once
#include "EntityAppearanceInterface.generated.h"

UINTERFACE(Blueprintable)
class UEntityAppearanceInterface:public UInterface
{
        GENERATED_UINTERFACE_BODY()
};

class IEntityAppearanceInterface
{
    GENERATED_IINTERFACE_BODY()

public:
    virtual void OnDeploy() = 0;
    virtual void OnDeath() = 0;
    virtual void OnAttack() = 0;
    virtual void OnDamage() = 0;
};
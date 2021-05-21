#pragma once
#include "DamageComponent.generated.h"


class AEntity;
UCLASS()
class UDamageComponent:public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnDamageEvent(int32 DamageValue, FTransform DamageTransform);
private:
	AEntity* ParentEntity;
};

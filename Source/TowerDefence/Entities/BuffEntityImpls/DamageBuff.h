#pragma once
#include "TowerDefence/Entities/Base/Buff.h"
#include "DamageBuff.generated.h"
class ANumerialWidget;
UCLASS(BlueprintType)
class UDamageBuff:public UBuffEntity
{
	GENERATED_BODY()
public:
	UDamageBuff(const FObjectInitializer& ObjectInitializer);
	virtual void OnChangeValue() override;
private:
};

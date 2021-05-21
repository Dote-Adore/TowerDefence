#pragma once
#include "TowerDefence/Entities/Base/Buff.h"
#include "DamageBuff.generated.h"

UCLASS(BlueprintType)
class UDamageBuff:public UBuffEntity
{
	GENERATED_BODY()
public:
	virtual void OnChangeValue() override;
};

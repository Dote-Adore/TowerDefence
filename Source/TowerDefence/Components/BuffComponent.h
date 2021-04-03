#pragma once
#include "CoreMinimal.h"
#include "BuffComponent.generated.h"

class UBuffEntity;
class AEntity;
struct FBuff;
UCLASS()
class UBuffComponent:public UActorComponent
{
    GENERATED_BODY()
public:
    void SetParentEntity(AEntity* Entity);
    void AddBuff(const FBuff& Buff);
    void RemoveBuff(int32 BuffID);
    
private:
    // 当前的Buff
    TMap<int32, UBuffEntity*> CurrentBuffs;
    TMap<int32, FTimerHandle> BuffTimeHandle;
    AEntity* ParentEntity;
};

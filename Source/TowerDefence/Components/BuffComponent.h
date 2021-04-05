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
    UBuffComponent(const FObjectInitializer& ObjectInitializer);
    virtual void BeginPlay() override;
    void AddBuff(const FBuff* Buff);
    
private:
    // 当前的Buff
    UPROPERTY(Transient)
    TMap<int32, UBuffEntity*> CurrentBuffs;
    TMap<int32, FTimerHandle> BuffTimeHandle;
    AEntity* ParentEntity;
    void RemoveBuff(int32 BuffID);
};

#pragma once
#include "Base/Entity.h"
#include "Turrent.generated.h"
UCLASS(BlueprintType)
class ATurrent: public AActor
{
    GENERATED_BODY()
public:
    void InitEntities(FEntityParams TurrentParams);
    void AddBuffs(const TArray<FBuff>& Buffs);
    virtual void Tick(float DeltaSeconds) override;

private:
    void CalculateBuffs(float DeltaSeconds);
    
    // 基础的参数
    FEntityParams BaseTurrent;
    // 当前加了buff后的参数
    FEntityParams CurrentTurrent;
    // 通过id快速找buff
    TMap<int32, FBuff> CurrentBuffs;
};
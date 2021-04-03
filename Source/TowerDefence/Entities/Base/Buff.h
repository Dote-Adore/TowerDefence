#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Buff.generated.h"
// 一个Buff原子
class AEntity;
USTRUCT(BlueprintType)
struct FBuffAtom
{
    GENERATED_BODY()
    // 目标更改的属性名
    UPROPERTY(EditAnywhere)
    FName TargetPropertyName;
    UPROPERTY(EditAnywhere)
    // 目标属性值
    float TargetValue;
};

UENUM()
enum class EBuffType
{
    // 倍率buff
    Magnification,
    // 加法buff
    Addition,
    // 持续性地加法buff,结束后将会保持状态
    ContinuouslyAddition
};
USTRUCT(BlueprintType)
struct FBuff:public FTableRowBase
{
    
    GENERATED_BODY()

    
    UPROPERTY(EditAnywhere)
    EBuffType BuffType;
    UPROPERTY(EditAnywhere)
    int32 BuffID;
    UPROPERTY(EditAnywhere)
    FName DisplayName;
    // 持续时间
    UPROPERTY(EditAnywhere)
    float duration;
    UPROPERTY(EditAnywhere)
    TArray<FBuffAtom> EffectParams;
};

// 每个buff有自己的一套实现
// int32 BuffID
DECLARE_DELEGATE_OneParam(FOnStopBuffDelegate, int32)
UCLASS()
class UBuffEntity : public UObject
{
    GENERATED_BODY()
public:
    UBuffEntity(const FObjectInitializer& ObjectInitializer);
    void Init(const FBuff& Buff, AEntity* Parent);
    virtual void ResetTimer();
    virtual void Start();
    virtual void BeginDestroy() override;
    FOnStopBuffDelegate OnStopBuffDelegate;
protected:
    virtual void Stop();

    // 如果类型是持续性质的影响，则这里表示其改变单次数值的间隔时间
    UPROPERTY(EditAnywhere)
    float ContinuouslyAdditionIntervals = 0.2;
private:
    void StartContinuously();
    FBuff MyBuff;
    AEntity* ParentEntity;
    FTimerHandle StopTimerHandle;
    TMap<FName, float> ChangedValueOffest;
    // 持续性改参数的buff的定时器
    FTimerHandle ContinuouslyTimerHandle;
};
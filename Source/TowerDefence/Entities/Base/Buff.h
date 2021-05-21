#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Buff.generated.h"
// 一个Buff原子
class AEntity;
class UBuffEntity;
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
    int32 BuffID;
    UPROPERTY(EditAnywhere)
    EBuffType BuffType;
    // 所使用到的buff实体class
    UPROPERTY(EditAnywhere)
    TSubclassOf<UBuffEntity> BuffEntityClass;
    UPROPERTY(EditAnywhere)
    FName DisplayName;
    // 持续时间
    UPROPERTY(EditAnywhere)
    float duration;
    UPROPERTY(EditAnywhere)
    TArray<FBuffAtom> EffectParams;
    FBuff();
};

// 每个buff有自己的一套实现
// int32 BuffID
DECLARE_DELEGATE_OneParam(FOnStopBuffDelegate, int32)
UCLASS(BlueprintType)
class UBuffEntity : public UObject
{
    GENERATED_BODY()
public:
    UBuffEntity(const FObjectInitializer& ObjectInitializer);
    void Init(const FBuff* Buff, AEntity* Parent);
    virtual void ResetTimer();
    virtual void BeginDestroy() override;
    FOnStopBuffDelegate OnStopBuffDelegate;
protected:

    // 重新计算buff后的攻击
    virtual void OnReCalculateOffestValue(float* CalculatedOffestValue, const FName& PropertyName);
    virtual void StartContinuously();
    // 如果类型是持续性质的影响，则这里表示其改变单次数值的间隔时间
    UPROPERTY(EditAnywhere)
    float ContinuouslyAdditionIntervals = 0.5;
    // 每一次更改value的时候就会调用这个函数，子类进行实现
    virtual void OnChangeValue();
    virtual void OnStop();

    AEntity* ParentEntity;
    TMap<FName, float> ChangedValueOffest;
    const FBuff* MyBuff;
private:
    void Stop();
    void Start();
    
    FTimerHandle StopTimerHandle;
    // 持续性改参数的buff的定时器
    FTimerHandle ContinuouslyTimerHandle;
};
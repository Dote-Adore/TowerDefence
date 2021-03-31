#pragma once
#include "CoreMinimal.h"

#include "UObject/ObjectMacros.h"

#include "Buff.generated.h"
// 一个Buff原子
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
    // 持续性地加法buff
    ContinuouslyAddition
};
USTRUCT(BlueprintType)
struct FBuff
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
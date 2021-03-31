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
    UPROPERTY()
    FName TargetPropertyName;
    UPROPERTY()
    // 目标属性值
    float TargetValue;
};

USTRUCT(BlueprintType)
struct FBuff
{

    UENUM()
    enum EBuffType
    {
        // 倍率buff
        Magnification,
        // 加法buff
        Addition,
        // 持续性地加法buff
        ContinuouslyAddition
    };
    
    GENERATED_BODY()

    
    UPROPERTY()
    EBuffType BuffType;
    UPROPERTY()
    int32 BuffID;
    UPROPERTY()
    FName DisplayName;
    // 持续时间
    UPROPERTY()
    float duration;
    UPROPERTY()
    TArray<FBuffAtom> EffectParams;
};
#include "Buff.h"
#include "Entity.h"

UBuffEntity::UBuffEntity(const FObjectInitializer& ObjectInitializer)
    :UObject(ObjectInitializer)
{
}

void UBuffEntity::Init(const FBuff& Buff, AEntity* Parent)
{
    ParentEntity =Parent;
    MyBuff  = Buff;
}

void UBuffEntity::ResetTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(StopTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(StopTimerHandle,this, &UBuffEntity::Stop, MyBuff.duration, false);
}

void UBuffEntity::Start()
{
    
    // 持续性的buff有单独一套
    if(MyBuff.BuffType == EBuffType::ContinuouslyAddition)
    {
        GetWorld()->GetTimerManager().SetTimer(ContinuouslyTimerHandle, this, &UBuffEntity::StartContinuously, ContinuouslyAdditionIntervals, true);
        // 如果buff的时间小于等于0，则说明这个buff是永久有效的
        if(MyBuff.duration>=0)
        {
            GetWorld()->GetTimerManager().SetTimer(StopTimerHandle,this, &UBuffEntity::Stop, MyBuff.duration, false);
        }
        return;
    }
    FEntityParams& CurrentEntityParams = ParentEntity->GetCurrentEntityParams();
    for(const FBuffAtom& BuffParam:MyBuff.EffectParams)
    {
        const FName& TargetChangedParmName = BuffParam.TargetPropertyName;
        // 目标要改变的数值
        FProperty* TargetChangdProperty = CurrentEntityParams.StaticStruct()->FindPropertyByName(TargetChangedParmName);
        float* TargetFloatValue = TargetChangdProperty->ContainerPtrToValuePtr<float>(CurrentEntityParams.StaticStruct());
        int32* TargetIntValue = TargetChangdProperty->ContainerPtrToValuePtr<int32>(CurrentEntityParams.StaticStruct());
        float BeforeValue;
        if(TargetFloatValue)
        {
            BeforeValue = *TargetFloatValue;
        }
        else
        {
            if(TargetIntValue)
            {
                BeforeValue = *TargetIntValue;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Can not Found Property %s in Base EntityParams"), *TargetChangedParmName.ToString());
                continue;
            }
        }
        float TargetChangedValue = 0;
        switch(MyBuff.BuffType)
        {
            case EBuffType::Addition:
                ChangedValueOffest.Add(TargetChangedParmName, BuffParam.TargetValue);
                TargetChangedValue = BeforeValue + BuffParam.TargetValue;
                break;
            // 乘法的话转换成加法，这样可以避免一些多个buff叠加的计算错误
            case EBuffType::Magnification:
                ChangedValueOffest.Add(TargetChangedParmName, BeforeValue*BuffParam.TargetValue);
                TargetChangedValue = BeforeValue + BeforeValue*BuffParam.TargetValue;
                break;
        }
        if(TargetFloatValue)
        {
            *TargetFloatValue = TargetChangedValue;
        }
        else
        {
            *TargetIntValue = TargetChangedValue;
        }
        
    }
    if(MyBuff.duration>=0)
    {
        GetWorld()->GetTimerManager().SetTimer(StopTimerHandle,this, &UBuffEntity::Stop, MyBuff.duration, false);
    }
}

void UBuffEntity::BeginDestroy()
{
    Super::BeginDestroy();
}


void UBuffEntity::Stop()
{
    if(MyBuff.BuffType == EBuffType::ContinuouslyAddition)
    {
        GetWorld()->GetTimerManager().ClearTimer(ContinuouslyTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(StopTimerHandle);
        return;
    }
    GetWorld()->GetTimerManager().ClearTimer(StopTimerHandle);
    FEntityParams& CurrentEntityParams = ParentEntity->GetCurrentEntityParams();
    for(const FBuffAtom& BuffParam:MyBuff.EffectParams)
    {
        const FName& TargetChangedParmName = BuffParam.TargetPropertyName;
        // 目标要改变的数值
        FProperty* TargetChangdProperty = CurrentEntityParams.StaticStruct()->FindPropertyByName(TargetChangedParmName);
        float* TargetFloatValue = TargetChangdProperty->ContainerPtrToValuePtr<float>(CurrentEntityParams.StaticStruct());
        int32* TargetIntValue = TargetChangdProperty->ContainerPtrToValuePtr<int32>(CurrentEntityParams.StaticStruct());
        float* OffestValue = ChangedValueOffest.Find(TargetChangedParmName);
        if(TargetFloatValue)
        {
            *TargetFloatValue = *TargetFloatValue - *OffestValue;
        }
        else if(TargetIntValue)
        {
            *TargetIntValue = *TargetIntValue - *OffestValue;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Can not Found Property %s in Base EntityParams"), *TargetChangedParmName.ToString());
            continue;
        }
    }
    // 发送消息
    OnStopBuffDelegate.ExecuteIfBound(MyBuff.BuffID);
}

void UBuffEntity::StartContinuously()
{
    FEntityParams& CurrentEntityParams = ParentEntity->GetCurrentEntityParams();
    for(const FBuffAtom& BuffParam:MyBuff.EffectParams)
    {
        float TargetOffsetValue = BuffParam.TargetValue/(MyBuff.duration/ContinuouslyAdditionIntervals);
        const FName& TargetChangedParmName = BuffParam.TargetPropertyName;
        // 目标要改变的数值
        FProperty* TargetChangdProperty = CurrentEntityParams.StaticStruct()->FindPropertyByName(TargetChangedParmName);
        float* TargetFloatValue = TargetChangdProperty->ContainerPtrToValuePtr<float>(CurrentEntityParams.StaticStruct());
        int32* TargetIntValue = TargetChangdProperty->ContainerPtrToValuePtr<int32>(CurrentEntityParams.StaticStruct());
        if(TargetFloatValue)
        {
            *TargetFloatValue += TargetOffsetValue;
            continue;
        }
        if(TargetIntValue)
        {
            *TargetIntValue += TargetOffsetValue;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Can not Found Property %s in Base EntityParams"), *TargetChangedParmName.ToString());
            continue;
        }
    }
}

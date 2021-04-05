#include "Buff.h"
#include "Entity.h"

FBuff::FBuff()
{
    BuffEntityClass = UBuffEntity::StaticClass();
}

UBuffEntity::UBuffEntity(const FObjectInitializer& ObjectInitializer)
    :UObject(ObjectInitializer)
{
}

void UBuffEntity::Init(const FBuff* Buff, AEntity* Parent)
{
    ParentEntity =Parent;
    MyBuff = Buff;
    Start();
}

void UBuffEntity::ResetTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(StopTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(StopTimerHandle,this, &UBuffEntity::Stop, MyBuff->duration, false);
}

void UBuffEntity::Start()
{
    UE_LOG(LogTemp, Display, TEXT("Start Buff, ID:%d, Effect Entity: %s"), MyBuff->BuffID, *ParentEntity->GetName());
    // 持续性的buff有单独一套,持续性的buff在buff结束后仍然保持数值不变
    if(MyBuff->BuffType == EBuffType::ContinuouslyAddition)
    {
        GetWorld()->GetTimerManager().SetTimer(ContinuouslyTimerHandle, this, &UBuffEntity::StartContinuously, ContinuouslyAdditionIntervals, true);
        // 如果buff的时间小于等于0，则说明这个buff是永久有效的
        if(MyBuff->duration>0)
        {
            GetWorld()->GetTimerManager().SetTimer(StopTimerHandle,this, &UBuffEntity::Stop, MyBuff->duration, false);
        }
        else if(FMath::IsNearlyEqual(MyBuff->duration, 0))
        {
            StartContinuously();
            Stop();
        }
        return;
    }
    FEntityParams& CurrentEntityParams = ParentEntity->GetCurrentEntityParams();
    for(const FBuffAtom& BuffParam:MyBuff->EffectParams)
    {
        const FName& TargetChangedParmName = BuffParam.TargetPropertyName;
        // 目标要改变的数值
        FProperty* TargetChangdProperty = CurrentEntityParams.StaticStruct()->FindPropertyByName(TargetChangedParmName);
        FIntProperty* IntProperty = CastField<FIntProperty>(TargetChangdProperty);
        FFloatProperty* FloatProperty =CastField<FFloatProperty>(TargetChangdProperty);
        float BeforeValue = 0;
        if(FloatProperty)
        {
            float* TargetFloatValue = TargetChangdProperty->ContainerPtrToValuePtr<float>(&CurrentEntityParams);
            BeforeValue = *TargetFloatValue;
        }
        else
        {
            if(IntProperty)
            {
                int32* TargetIntValue = TargetChangdProperty->ContainerPtrToValuePtr<int32>(&CurrentEntityParams);
                BeforeValue = *TargetIntValue;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Can not Found Property %s in Base EntityParams"), *TargetChangedParmName.ToString());
                continue;
            }
        }
        float TargetChangedOffest = 0;
        switch(MyBuff->BuffType)
        {
            case EBuffType::Addition:
                TargetChangedOffest = BuffParam.TargetValue;
                OnReCalculateOffestValue(&TargetChangedOffest, TargetChangedParmName);
                ChangedValueOffest.Add(TargetChangedParmName, TargetChangedOffest);
                break;
            // 乘法的话转换成加法，这样可以避免一些多个buff叠加的计算错误
            case EBuffType::Magnification:
                TargetChangedOffest = BeforeValue*BuffParam.TargetValue;
                OnReCalculateOffestValue(&TargetChangedOffest, TargetChangedParmName);
                ChangedValueOffest.Add(TargetChangedParmName, TargetChangedOffest);
                break;
        }
        float TargetChangedValue = BeforeValue + TargetChangedOffest;
        if(FloatProperty)
        {
            FloatProperty->SetPropertyValue_InContainer(&CurrentEntityParams, TargetChangedValue);
        }
        else
        {
            IntProperty->SetPropertyValue_InContainer(&CurrentEntityParams, TargetChangedValue);
        }
        
    }
    if(MyBuff->duration>0)
    {
        GetWorld()->GetTimerManager().SetTimer(StopTimerHandle,this, &UBuffEntity::Stop, MyBuff->duration, false);
    }
    else if(FMath::IsNearlyEqual(MyBuff->duration, 0))
    {
        Stop();
    }
}

void UBuffEntity::OnReCalculateOffestValue(float* CalculatedOffestValue, const FName& PropertyName)
{
}

void UBuffEntity::BeginDestroy()
{
    Super::BeginDestroy();
}


void UBuffEntity::Stop()
{
    UE_LOG(LogTemp, Display, TEXT("Remove Buff, ID:%d, Effect Entity: %s "), MyBuff->BuffID, *ParentEntity->GetName());
    this->MarkPendingKill();
    if(MyBuff->BuffType == EBuffType::ContinuouslyAddition)
    {
        GetWorld()->GetTimerManager().ClearTimer(ContinuouslyTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(StopTimerHandle);
        OnStopBuffDelegate.ExecuteIfBound(MyBuff->BuffID);
        return;
    }
    GetWorld()->GetTimerManager().ClearTimer(StopTimerHandle);
    FEntityParams& CurrentEntityParams = ParentEntity->GetCurrentEntityParams();
    for(const FBuffAtom& BuffParam:MyBuff->EffectParams)
    {
        const FName& TargetChangedParmName = BuffParam.TargetPropertyName;
        // 目标要改变的数值
        FProperty* TargetChangdProperty = CurrentEntityParams.StaticStruct()->FindPropertyByName(TargetChangedParmName);
        FIntProperty* IntProperty = CastField<FIntProperty>(TargetChangdProperty);
        FFloatProperty* FloatProperty =CastField<FFloatProperty>(TargetChangdProperty);
        float* OffestValue = ChangedValueOffest.Find(TargetChangedParmName);
        if(FloatProperty)
        {
            float* TargetFloatValue = TargetChangdProperty->ContainerPtrToValuePtr<float>(&CurrentEntityParams);
            *TargetFloatValue = *TargetFloatValue - *OffestValue;
        }
        else if(IntProperty)
        {
            int32* TargetIntValue = TargetChangdProperty->ContainerPtrToValuePtr<int32>(&CurrentEntityParams);
            *TargetIntValue = *TargetIntValue - *OffestValue;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Can not Found Property %s in Base EntityParams"), *TargetChangedParmName.ToString());
            continue;
        }
    }
    // 发送消息
    OnStopBuffDelegate.ExecuteIfBound(MyBuff->BuffID);
}

void UBuffEntity::StartContinuously()
{
    UE_LOG(LogTemp, Display, TEXT("Continuous Buff, ID:%d, Effect Entity: %s, Effect Param: "), MyBuff->BuffID, *ParentEntity->GetName());
    FEntityParams& CurrentEntityParams = ParentEntity->GetCurrentEntityParams();
    for(const FBuffAtom& BuffParam:MyBuff->EffectParams)
    {
        float TargetOffsetValue;
        if(MyBuff->duration>0)
            TargetOffsetValue = BuffParam.TargetValue/(MyBuff->duration/ContinuouslyAdditionIntervals);
        else
            TargetOffsetValue = BuffParam.TargetValue;
        const FName& TargetChangedParmName = BuffParam.TargetPropertyName;
        // 目标要改变的数值
        FProperty* TargetChangdProperty = CurrentEntityParams.StaticStruct()->FindPropertyByName(TargetChangedParmName);
        FIntProperty* IntProperty = CastField<FIntProperty>(TargetChangdProperty);
        FFloatProperty* FloatProperty =CastField<FFloatProperty>(TargetChangdProperty);
        if(FloatProperty)
        {
            float* BeforeValue = TargetChangdProperty->ContainerPtrToValuePtr<float>(&CurrentEntityParams);
            FloatProperty->SetPropertyValue_InContainer(&CurrentEntityParams, *BeforeValue+TargetOffsetValue);
            continue;
        }
        if(IntProperty)
        {
            int32* BeforeValue = TargetChangdProperty->ContainerPtrToValuePtr<int32>(&CurrentEntityParams);
            IntProperty->SetPropertyValue_InContainer(&CurrentEntityParams, *BeforeValue+TargetOffsetValue);
            //*TargetIntValue += TargetOffsetValue;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Can not Found Property %s in Base EntityParams"), *TargetChangedParmName.ToString());
        }
    }
}

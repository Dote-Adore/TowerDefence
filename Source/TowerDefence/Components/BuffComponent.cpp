#include "BuffComponent.h"
#include "TowerDefence/Entities/Base/Buff.h"
#include "TowerDefence/Entities/Base/Entity.h"

UBuffComponent::UBuffComponent(const FObjectInitializer& ObjectInitializer)
    :UActorComponent(ObjectInitializer)
{
  
}

void UBuffComponent::BeginPlay()
{
    AActor* Owner = GetOwner();
    ParentEntity = Cast<AEntity>(Owner);
    check(ParentEntity);
}

void UBuffComponent::AddBuff(const FBuff* Buff)
{
    UBuffEntity** ExistedBuff =  CurrentBuffs.Find(Buff->BuffID);
    if(ExistedBuff&&*ExistedBuff)
    {
        (*ExistedBuff)->ResetTimer();
    }
    else
    {
        UBuffEntity*  NewBuff = NewObject<UBuffEntity>(this, Buff->BuffEntityClass);
        NewBuff->Init(Buff, ParentEntity);
        // 监听buff停止的消息，然后将其remove掉
        NewBuff->OnStopBuffDelegate.BindUObject(this, &UBuffComponent::RemoveBuff);
    }
}

void UBuffComponent::RemoveBuff(int32 BuffID)
{
    CurrentBuffs.Remove(BuffID);
}

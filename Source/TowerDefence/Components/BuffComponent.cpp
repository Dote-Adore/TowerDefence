#include "BuffComponent.h"
#include "TowerDefence/Entities/Base/Buff.h"
#include "TowerDefence/Entities/Base/Entity.h"

UBuffComponent::UBuffComponent(const FObjectInitializer& ObjectInitializer)
    :UActorComponent(ObjectInitializer)
{
  
}

void UBuffComponent::BeginPlay()
{
    Super::BeginPlay();
    AActor* Owner = GetOwner();
    ParentEntity = Cast<AEntity>(Owner);
    check(ParentEntity);
}
 
void UBuffComponent::AddBuff(const FBuff* Buff)
{
    UBuffEntity** ExistedBuff =  CurrentBuffs.Find(Buff->BuffID);
    if(ExistedBuff&&(*ExistedBuff)!=nullptr)
    {
        (*ExistedBuff)->ResetTimer();
    }
    else
    {
        UBuffEntity*  NewBuff = NewObject<UBuffEntity>(this, Buff->BuffEntityClass);
        // 监听buff停止的消息，然后将其remove掉
        CurrentBuffs.Add(Buff->BuffID, NewBuff);
        NewBuff->OnStopBuffDelegate.BindUObject(this, &UBuffComponent::RemoveBuff);
        NewBuff->Init(Buff, ParentEntity);
        
    }
}

void UBuffComponent::RemoveBuff(int32 BuffID)
{
    UE_LOG(LogTemp, Display, TEXT("[%s][BuffComponent]: Remove Buff %d"),*ParentEntity->GetName(), BuffID);
    UBuffEntity** BuffEntity = CurrentBuffs.Find(BuffID);
    if(BuffEntity&& *BuffEntity!=nullptr)
    {
        (*BuffEntity)->MarkPendingKill();
        (*BuffEntity) = nullptr;
    }
    CurrentBuffs.Remove(BuffID);
    
}

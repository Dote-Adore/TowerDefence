#include "BuffComponent.h"
#include "TowerDefence/Entities/Base/Buff.h"
#include "TowerDefence/Entities/Base/Entity.h"

void UBuffComponent::SetParentEntity(AEntity* Entity)
{
    ParentEntity = Entity;
}

void UBuffComponent::AddBuff(const FBuff& Buff)
{
    UBuffEntity** ExistedBuff =  CurrentBuffs.Find(Buff.BuffID);
    if(ExistedBuff&&*ExistedBuff)
    {
        (*ExistedBuff)->ResetTimer();
    }
    else
    {
        UBuffEntity*  NewBuff = NewObject<UBuffEntity>(this, UBuffEntity::StaticClass());
        NewBuff->Init(Buff, ParentEntity);
        NewBuff->Start();
        // NewBuff->OnStopBuffDelegate.CreateRaw(this, &UBuffComponent::RemoveBuff);
    }
}

void UBuffComponent::RemoveBuff(int32 BuffID)
{
    CurrentBuffs.Remove(BuffID);
}

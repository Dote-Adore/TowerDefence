#include "Turrent.h"

void ATurrent::InitEntities(FEntity TurrentParams)
{
    this->BaseTurrent = TurrentParams;
}

void ATurrent::AddBuffs(const TArray<FBuff>& Buffs)
{
    for(auto Buff: Buffs)
    {
        FBuff* CurrentBuff = CurrentBuffs.Find(Buff.BuffID);
        // 如果已经有这个buff了，则刷新时间
        if(CurrentBuff)
        {
            CurrentBuff->duration = Buff.duration;
        }
        else
        {
            CurrentBuffs.Add(Buff.BuffID, Buff);
        }
    }
}

void ATurrent::Tick(float DeltaSeconds)
{
    CalculateBuffs(DeltaSeconds);
}

void ATurrent::CalculateBuffs(float DeltaSeconds)
{
    for(auto& Pair:CurrentBuffs)
    {
        FBuff& Buff = Pair.Value;
        if(Buff.duration <= 0)
        {
            continue;
        }
    }
}
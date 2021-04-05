#include "GlobalConfig.h"

const FBuff* UGlobalConfig::FindBuffByID(int32 ID)
{
    if(LoadedBuffs.Find(ID))
        return *LoadedBuffs.Find(ID);
    UDataTable* Datas = BuffDataTable.LoadSynchronous();
    check(Datas);
    FBuff* TargetBuff = Datas->FindRow<FBuff>(FName(*FString::FromInt(ID)), GET_MEMBER_NAME_STRING_CHECKED(FBuff, BuffID));
    LoadedBuffs.Add(ID, TargetBuff);
    return TargetBuff;
}

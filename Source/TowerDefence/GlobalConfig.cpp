#include "GlobalConfig.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/DataTable.h"
#include "Level/LevelInfomation.h"
#include "TowerDefence/Entities/Base/Buff.h"

const FBuff* UGlobalConfig::FindBuffByID(int32 ID)
{
    if(LoadedBuffs.Find(ID))
        return *LoadedBuffs.Find(ID);
    UDataTable* Datas = BuffDataTable.LoadSynchronous();
    check(Datas);
    FBuff* TargetBuff = Datas->FindRow<FBuff>(FName(*FString::FromInt(ID)), GET_MEMBER_NAME_STRING_CHECKED(FBuff, BuffID));
    if(TargetBuff!=nullptr)
    {
        LoadedBuffs.Add(ID, TargetBuff);
    }
    return TargetBuff;
}

const UGlobalConfig::AllLevelsMap UGlobalConfig::GetAllLevels() const
{
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> AssetDatas;
    AssetRegistryModule.Get().GetAssetsByPath(FName(TargetLevelsPath), AssetDatas);
    for(const FAssetData& AssetData: AssetDatas)
    {
        // 如果这个Assetdata是我想要的，就放到load里面
        if(AssetData.GetClass()->IsChildOf(ULevelInfomation::StaticClass()))
        {
            ULevelInfomation* TargetLevel = Cast<ULevelInfomation>(AssetData.GetAsset());
            LoadedLevels.FindOrAdd(TargetLevel->GetFName(), TargetLevel);
        }
    }
    return LoadedLevels;
}

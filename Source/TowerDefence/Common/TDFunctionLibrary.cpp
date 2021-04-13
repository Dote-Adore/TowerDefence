#include "TDFunctionLibrary.h"
#include "TowerDefence/GlobalConfig.h"
#include "Engine/DataTable.h"

const TMap<FName, ULevelInfomation*> UTDFunctionLibrary::GetAllLevelInfomations()
{
	const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
	return Config->GetAllLevels();
}

TArray<FEntityParams> UTDFunctionLibrary::GetAllTurrentParams()
{
	const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
	UDataTable* TurrentDataTable = Config->TurrentDataTable.LoadSynchronous();
	check(TurrentDataTable);
	TArray<FEntityParams*> FoundParams;
	TurrentDataTable->GetAllRows<FEntityParams>("EntityID", FoundParams);
	TArray<FEntityParams> Res;
	for(auto Item:FoundParams)
	{
		Res.Add(*Item);
	}
	return Res;
}

FTurrentAdditionalInfo UTDFunctionLibrary::FindTurrentAdditionalInfoByID(int32 EntityID)
{
	const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
	UDataTable* TurrentAddDataTable = Config->TurrentAdditialTable.LoadSynchronous();
	check(TurrentAddDataTable);
	FTurrentAdditionalInfo* Res = TurrentAddDataTable->FindRow<FTurrentAdditionalInfo>(FName(*FString::FromInt(EntityID)), "EntityID");
	check(Res);
	return *Res;
}

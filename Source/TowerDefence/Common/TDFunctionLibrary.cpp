#include "TDFunctionLibrary.h"

#include "TowerDefence/GlobalConfig.h"

const TMap<FName, ULevelInfomation*> UTDFunctionLibrary::GetAllLevelInfomations()
{
	const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
	return Config->GetAllLevels();
}

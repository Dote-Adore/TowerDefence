#pragma once
// 关卡存档，存放关卡信息，包括已经通过的关卡，每个关卡通关信息等

#include "GameFramework/SaveGame.h"
#include "LevelArchive.generated.h"
USTRUCT()
struct FLevelPassedSave
{
	GENERATED_BODY()
	// 过关星级，一星到五星
	UPROPERTY()
	int32 StarNum;
	
};


UCLASS()
class ULevelArchive:public USaveGame
{
	GENERATED_BODY()
public:
	// UPROPERTY()
	// int32 Gained;
	// 每个关卡最佳过关的存档
	UPROPERTY()
	TMap<int32, FLevelPassedSave> BestLevelsPassedInfo;
	
};

#pragma once
// 关卡存档，存放关卡信息，包括已经通过的关卡，每个关卡通关信息等

#include "GameFramework/SaveGame.h"
#include "LevelTaskArchive.generated.h"

UENUM()
enum class ELevelTaskState
{
	Locked,
	UnLock,
	Finished
};


USTRUCT()
struct FLevelPassedSave
{
	GENERATED_BODY()
	// 过关星级，一星到五星
	UPROPERTY()
	int32 StarNum;
	
};

USTRUCT(BlueprintType)
struct FEachLevelTaskSavedData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELevelTaskState TaskState;
	UPROPERTY(BlueprintReadWrite)
	int32 Test;

};

UCLASS()
class ULevelTaskArchive:public USaveGame
{
	GENERATED_BODY()
public:
	// // 解锁的所有关卡
	// UPROPERTY(BlueprintReadWrite)
	// TSet<int32> UnLockedLevels;
	// // 没有解锁的关卡列表
	// UPROPERTY(BlueprintReadWrite)
	// TSet<int32> LockedLevels;
	// // 已经完成的关卡列表
	// UPROPERTY(BlueprintReadWrite)
	// TSet<int32> FinishedLevels;
	// 已经通关的其它参数
	// UPROPERTY(BlueprintReadWrite)
	// TMap<int32, FLevelPassedSave> PassedLevelSavedInfo;
	// 所有的关卡状态
	UPROPERTY(BlueprintReadWrite)
	TMap<int32, FEachLevelTaskSavedData> LevelTaskStates;
	
};

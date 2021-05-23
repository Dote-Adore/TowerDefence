#pragma once
#include "TowerDefence/Datas/LevelTaskData.h"
#include "TowerDefence/Datas/SaveDatas/LevelTaskArchive.h"
#include "LevelTaskSystem.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LevelTaskSystem, Log, All);

class UArchiveSystem;
// struct FEachLevelTaskSavedData;

USTRUCT(BlueprintType)
struct FLevelTaskItem
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FLevelTaskData ConfigData;
	UPROPERTY(BlueprintReadWrite)
	FEachLevelTaskSavedData TaskSavedData;
	
};

UCLASS()
class TOWERDEFENCE_API ULevelTaskSystem:public UGameInstanceSubsystem
{
	GENERATED_BODY()
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UFUNCTION(BlueprintCallable)
	TArray<FLevelTaskItem> GetAllLevelTasks();
	UFUNCTION(BlueprintCallable)
	void FinishLevel(int32 LevelID);
	UFUNCTION(BlueprintCallable)
	void StartTask(FLevelTaskItem TargetLevelTaskItem, TSet<int32> UsedCharacterID);
private:
	UPROPERTY(Transient)
	UArchiveSystem* ArchiveSystem;
	UPROPERTY(Transient)
	TMap<int32, FLevelTaskData> TaskConfig;

	// 开始关卡
	void LoadTaskConfig();
	void SettlementReward(const FFinishReward& Reward);
};

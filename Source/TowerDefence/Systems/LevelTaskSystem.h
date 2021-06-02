#pragma once
#include "TowerDefence/Datas/LevelTaskData.h"
#include "TowerDefence/Datas/SaveDatas/LevelTaskArchive.h"
#include "TowerDefence/Entities/Base/Entity.h"

#include "LevelTaskSystem.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LevelTaskSystem, Log, All);

class ATurrent;
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
public:
	GENERATED_BODY()
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UFUNCTION(BlueprintCallable)
	TArray<FLevelTaskItem> GetAllLevelTasks();


	// 战斗关卡相关------
	UFUNCTION(BlueprintCallable)
	FFinishReward FinishLevel();
	UFUNCTION(BlueprintCallable)
	void StartTask(FLevelTaskItem TargetLevelTaskItem, TSet<int32> UsedCharacterID);

	// 获取所有可以用到的角色
	UFUNCTION(BlueprintCallable)
	TArray<FEntityParams> GetAllCanUsedCharacters();
	UFUNCTION(BlueprintCallable)
	FLevelTaskItem GetCurrentLevelTaskItem();
	UFUNCTION(BlueprintCallable)
	void RetryCurrentLevel();
	
	//  ---------------
private:
	UPROPERTY(Transient)
	UArchiveSystem* ArchiveSystem;
	UPROPERTY(Transient)
	TMap<int32, FLevelTaskData> TaskConfig;


	// 战斗关卡相关------
	UPROPERTY(Transient)
	FLevelTaskItem CurrentLevelTaskItem;
	UPROPERTY(Transient)
	TSet<int32> CurrentUsedCharacterID;
	//  ---------------
	
	// 开始关卡
	void LoadTaskConfig();
	void SettlementReward(const FFinishReward& Reward, FFinishReward& StatisticcalReward);
};

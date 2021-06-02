#include "LevelTaskSystem.h"

#include "TowerDefence/Datas/SaveDatas/LevelTaskArchive.h"
#include "Engine/DataTable.h"
#include "TowerDefence/GlobalConfig.h"
#include "ArchiveSystem.h"
#include "CharacterSystem.h"
#include "PackageSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Datas/LevelTaskData.h"
#include "TowerDefence/Datas/SaveDatas/UserArchive.h"
#include "TowerDefence/Level/LevelInfomation.h"

DEFINE_LOG_CATEGORY(LevelTaskSystem)


void ULevelTaskSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
	LoadTaskConfig();
}

TArray<FLevelTaskItem> ULevelTaskSystem::GetAllLevelTasks()
{
	ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
	TArray<FLevelTaskItem> Res;
	bool NeedSave = false;
	for(auto TaskItem:TaskConfig)
	{
		int32 ID = TaskItem.Key;
		FLevelTaskItem TempItem;
		TempItem.ConfigData = TaskItem.Value;
		FEachLevelTaskSavedData* CurrentLevelTaskSavedPtr = ArchiveSystem->GetLevelArchive()->LevelTaskStates.Find(ID);
		UE_LOG(LevelTaskSystem, Display, TEXT("ID:%d, Item.LevelName: %s"), ID, *(TaskItem.Value.LevelName).ToString());
		if(CurrentLevelTaskSavedPtr == nullptr)
		{
			NeedSave = true;
			// 如果该关卡没有前置条件，则说明直接是解锁的
			if(TaskItem.Value.UnlockRequirment.FinishedLevels.Num() == 0)
			{
				TempItem.TaskSavedData.TaskState = ELevelTaskState::UnLock;
			}
			else
			{
				TempItem.TaskSavedData.TaskState = ELevelTaskState::Locked;
			}
			FEachLevelTaskSavedData TempSavedData;
			TempSavedData = TempItem.TaskSavedData;
			ArchiveSystem->GetLevelArchive()->LevelTaskStates.Add(ID, TempSavedData);
		}
		else
		{
			// 如果没有解锁，则先要看一下是不是前置条件满足了,满足了就给他解锁
			if(CurrentLevelTaskSavedPtr->TaskState == ELevelTaskState::Locked)
			{
				auto AllNeedFinishedLevels = TaskItem.Value.UnlockRequirment.FinishedLevels;
				// 看一看前置条件是否都是完成的
				for(auto LevelID:AllNeedFinishedLevels)
				{
					FEachLevelTaskSavedData* RequestmentLevelTaskSavedPtr = ArchiveSystem->GetLevelArchive()->LevelTaskStates.Find(LevelID);
					//如果有一个关卡没有完成，则就lock掉他
					if(RequestmentLevelTaskSavedPtr == nullptr || RequestmentLevelTaskSavedPtr->TaskState != ELevelTaskState::Finished)
					{
						CurrentLevelTaskSavedPtr->TaskState = ELevelTaskState::Locked;
						break;
					}
					CurrentLevelTaskSavedPtr->TaskState = ELevelTaskState::UnLock;
				}
				// 需要保存
				if(CurrentLevelTaskSavedPtr->TaskState != ELevelTaskState::Locked)
				{
					NeedSave = true;
				}
			}
				TempItem.TaskSavedData = *CurrentLevelTaskSavedPtr;
		}
		Res.Add(TempItem);
	}
	// 需要保存，则保存
	if(NeedSave == true)
	{
		ArchiveSystem->SaveArchive();
	}
	return Res;
}

FFinishReward ULevelTaskSystem::FinishLevel()
{
	int32 LevelID = CurrentLevelTaskItem.ConfigData.ID;
	const FLevelTaskData* LevelTaskData =TaskConfig.Find(LevelID);
	ensureMsgf(LevelTaskData, TEXT("Can not Find LevelTaskConfig in Target ID %d!"), LevelID);
	// 进行结算奖励
	FEachLevelTaskSavedData* BeforeTaskLevelSavedData = ArchiveSystem->GetLevelArchive()->LevelTaskStates.Find(LevelID);
	ensureMsgf(BeforeTaskLevelSavedData, TEXT("Can not Find LevelTaskArchive in Target ID %d!"), LevelID);
	// 如果之前是没有通关的，则给与第一次通关的奖励
	FFinishReward ResultReward;
	if(BeforeTaskLevelSavedData->TaskState == ELevelTaskState::UnLock)
	{
		// 首次通关结算奖励
		SettlementReward(LevelTaskData->FirstFinishRewards, ResultReward);
		
	}
	// 正常的奖励结算
	SettlementReward(LevelTaskData->NormalRewards, ResultReward);
	// 更改条件为关卡通关完成
	BeforeTaskLevelSavedData->TaskState = ELevelTaskState::Finished;
	ArchiveSystem->SaveArchive();
	return ResultReward;
}

void ULevelTaskSystem::StartTask(FLevelTaskItem TargetLevelTaskItem, TSet<int32> UsedCharacterID)
{
	CurrentLevelTaskItem = TargetLevelTaskItem;
	CurrentUsedCharacterID = UsedCharacterID;

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), TargetLevelTaskItem.ConfigData.TargetLevelInfo->BackgroundWorld);
}

TArray<FEntityParams> ULevelTaskSystem::GetAllCanUsedCharacters()
{
	// 计算得到所有的用到的部署角色
	UCharacterSystem* CharacterSys = GetGameInstance()->GetSubsystem<UCharacterSystem>();
	check(CharacterSys);
	TArray<FEntityParams> Res;
	for(auto ID: CurrentUsedCharacterID)
	{
		Res.Add(CharacterSys->GetEntityParam(ID));
	}
	return Res;
}

FLevelTaskItem ULevelTaskSystem::GetCurrentLevelTaskItem()
{
	return CurrentLevelTaskItem;
}

void ULevelTaskSystem::RetryCurrentLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), CurrentLevelTaskItem.ConfigData.TargetLevelInfo->BackgroundWorld);
}

void ULevelTaskSystem::LoadTaskConfig()
{
	TaskConfig.Empty();
	const UGlobalConfig* GlobalConfig = GetDefault<UGlobalConfig>();
	UDataTable* TaskDataTable = GlobalConfig->LevelTaskDataTable.LoadSynchronous();
	if(!TaskDataTable)
	{
		UE_LOG(LevelTaskSystem, Error, TEXT("Can not find LevelTaskDataTable DataTable in GlobalConfig!"));
		return;
	}
	TArray<FLevelTaskData*> AllDataArray;
	TaskDataTable->GetAllRows<FLevelTaskData>(GET_MEMBER_NAME_STRING_CHECKED(FLevelTaskData, ID), AllDataArray);
	for(auto TaskItem:AllDataArray)
	{
		TaskConfig.Add(TaskItem->ID, *TaskItem);
	}
}

void ULevelTaskSystem::SettlementReward(const FFinishReward& Reward, FFinishReward& StatisticcalReward)
{
	auto CurrentPackageSystem = GetGameInstance()->GetSubsystem<UPackageSystem>();
	for(auto FoodItem: Reward.Foods)
	{
		StatisticcalReward.Foods.FindOrAdd(FoodItem.Key, FoodItem.Value);
		CurrentPackageSystem->AddFoodItem(FoodItem.Key, FoodItem.Value);
	}
	for(auto DevelopItem: Reward.DevelopItems)
	{
		StatisticcalReward.DevelopItems.FindOrAdd(DevelopItem.Key, DevelopItem.Value);
		CurrentPackageSystem->AddDevelopItem(DevelopItem.Key, DevelopItem.Value);
	}
	ArchiveSystem->GetUserArchive()->GameCoinNum+=Reward.GameCoin;
	ArchiveSystem->GetUserArchive()->PaidCoinNum+=Reward.PaidCoin;
	if(Reward.GainedNewCharacterID!=0)
	{
		GetGameInstance()->GetSubsystem<UCharacterSystem>()->AddNewCharacter(Reward.GainedNewCharacterID);
		StatisticcalReward.GainedNewCharacterID = Reward.GainedNewCharacterID;
	}
	StatisticcalReward.GameCoin+=Reward.GameCoin;
	StatisticcalReward.PaidCoin+=Reward.GameCoin;
}

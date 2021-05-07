#include "CharacterSystem.h"
#include "TowerDefence/Datas/SaveDatas/UserArchive.h"
#include "ArchiveSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/GlobalConfig.h"
#include "TowerDefence/Common/TDFunctionLibrary.h"

DEFINE_LOG_CATEGORY(CharacterSystem)


void UCharacterSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
	LoadAllRankConfig();
	AllEntityParamArray = UTDFunctionLibrary::GetAllTurrentParams();
}

FCharacterSavedInfo UCharacterSystem::UpgradeCharacter(int32 ID, int32 UpgradePoints, bool JustTest /*= true */)
{
	UUserArchive* UserArchive;
	UserArchive = ArchiveSystem->GetUserArchive();
	FCharacterSavedInfo* TargetChangedCharacter = UserArchive->OwnedCharacters.Find(ID);
	if(TargetChangedCharacter == nullptr)
	{
		UE_LOG(CharacterSystem, Error, TEXT("Target character id '%d' does not exist by user! upgrade failed!"), ID);
		return FCharacterSavedInfo();
	}
	// 如果单纯的只是测试，则是使用临时的数据，而不更改元数据
	if(JustTest)
	{
		TargetChangedCharacter = new FCharacterSavedInfo(*TargetChangedCharacter);
	}
	// 获取到升级策略配置信息
	FCharacterRankConfig** CurrentRankConfigPtr = RankConfigs.Find(ID);
	if(CurrentRankConfigPtr == nullptr)
	{
		UE_LOG(CharacterSystem, Error, TEXT("Can not Get target Character '%d' Rank Config!"), ID);
		return *TargetChangedCharacter;
	}
	FCharacterRankConfig* CurrentRankConfig = *CurrentRankConfigPtr;
	// 如果已经是最顶了，则不进行升级
	if(TargetChangedCharacter->CurrentLevel == CurrentRankConfig->MaxLevel)
	{
		UE_LOG(CharacterSystem, Error, TEXT("Can not upgrade Character, because is max Level"));
		return *TargetChangedCharacter;
	}
	// 如果没有升级过，则初始化一下默认的升级点数
	if(TargetChangedCharacter->TotalUpgradeNextNeededPoints == 0)
	{
		TargetChangedCharacter->TotalUpgradeNextNeededPoints = CurrentRankConfig->InitialUpgradePoints;
	}
	
	// 开始进行升级计算
	while(UpgradePoints > 0)
	{
		UpgradePoints -=
			(TargetChangedCharacter->TotalUpgradeNextNeededPoints - TargetChangedCharacter->LeftUpgradePoints);
		// 如果升级点数不够升级的话，直接更新剩余点数，不进行升级
		if(UpgradePoints<0)
		{
			TargetChangedCharacter->LeftUpgradePoints =
				TargetChangedCharacter->TotalUpgradeNextNeededPoints + UpgradePoints;
			break;
		}
		// 升级
		TargetChangedCharacter->CurrentLevel++;
		TargetChangedCharacter->TotalUpgradeNextNeededPoints += CurrentRankConfig->InitialUpgradePoints;
		TargetChangedCharacter->Attack += CurrentRankConfig->AttackIncrease;
		TargetChangedCharacter->Defence  += CurrentRankConfig->DefenceIncrease;
		TargetChangedCharacter->MaxHP +=CurrentRankConfig->MaxHpIncrease;
		TargetChangedCharacter->LeftUpgradePoints = 0;
		// 如果到达最高等级，则终止升级
		if(TargetChangedCharacter->CurrentLevel == CurrentRankConfig->MaxLevel)
		{
			break;
		}
	}
	// 存下档
	if(JustTest == false)
	{
		ArchiveSystem->SaveArchive();
	}
	return *TargetChangedCharacter;
}

TArray<UCharacterEntryItem*> UCharacterSystem::GetAllOwnedCharacters()
{
	TArray<UCharacterEntryItem*> Reval;
	for(auto InCharacter: ArchiveSystem->GetUserArchive()->OwnedCharacters)
	{
		UCharacterEntryItem* EntryItem = NewObject<UCharacterEntryItem>(this);
		EntryItem->SavedInfo = InCharacter.Value;
		EntryItem->AdditionalInfo = GetCharacterAdditionalInfo(InCharacter.Key);
		EntryItem->EntityParams = GetEntityParam(InCharacter.Key);
		Reval.Add(EntryItem);	
	}
	return Reval;
}

const FTurrentAdditionalInfo UCharacterSystem::GetCharacterAdditionalInfo(int32 ID)
{
	return UTDFunctionLibrary::FindTurrentAdditionalInfoByID(ID);
}

const FEntityParams UCharacterSystem::GetEntityParam(int32 ID)
{
	for(auto EntityParam: AllEntityParamArray)
	{
		if(EntityParam.EntityID == ID)
			return EntityParam;
	}
	return FEntityParams();
}

void UCharacterSystem::AddNewCharacter(int32 ID)
{
	const FEntityParams& TargetEntityParam = GetEntityParam(ID);
	const auto TargetRankConfig = RankConfigs.Find(ID);
	FCharacterSavedInfo NewCharacter;
	NewCharacter.TargetTurretId = ID;
	NewCharacter.CurrentLevel = 1;
	NewCharacter.MaxHP = TargetEntityParam.MaxHP;
	NewCharacter.TotalUpgradeNextNeededPoints = (*TargetRankConfig)->InitialUpgradePoints;
	NewCharacter.LeftUpgradePoints = 0;
	NewCharacter.Attack = TargetEntityParam.Attack;
	NewCharacter.Defence = TargetEntityParam.Defence;

	ArchiveSystem->GetUserArchive()->OwnedCharacters.Add(ID, NewCharacter);
	ArchiveSystem->SaveArchive();
}

void UCharacterSystem::LoadAllRankConfig()
{
	RankConfigs.Empty();
	const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
	UDataTable* RankDataTable =  Config->CharacterRankConfigTable.LoadSynchronous();
	if(!RankDataTable)
	{
		UE_LOG(CharacterSystem, Error, TEXT("Can not find Character Rank Config DataTable in GlobalConfig!"));
		return;
	}
	TArray<FCharacterRankConfig*> RankConfigArray;
	RankDataTable->GetAllRows<FCharacterRankConfig>(GET_MEMBER_NAME_STRING_CHECKED(FCharacterRankConfig, TurretID), RankConfigArray);
	for(auto RankItem:RankConfigArray)
	{
		RankConfigs.Add(RankItem->TurretID, RankItem);
	}
}

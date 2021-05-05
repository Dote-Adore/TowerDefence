#pragma once
#include "TowerDefence/Datas/CharacterDatas.h"
#include "TowerDefence/Entities/Base/Entity.h"
#include "TowerDefence/Datas/TurrentAdditional.h"
#include "CharacterSystem.generated.h"

// 所有的关于角色相关的信息
DECLARE_LOG_CATEGORY_EXTERN(CharacterSystem, Log, All);

class UArchiveSystem;
// struct FTurrentAdditionalInfo;
// struct FEntityParams;


// 传给UI界面需要用的object类，
UCLASS(BlueprintType)
class UCharacterEntryItem:public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FCharacterSavedInfo SavedInfo;
	UPROPERTY(BlueprintReadOnly)
	FTurrentAdditionalInfo AdditionalInfo;
	UPROPERTY(BlueprintReadOnly)
	FEntityParams EntityParams;
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* BindWidget;
};
UCLASS()
class TOWERDEFENCE_API UCharacterSystem:public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// 升级角色，输入目标角色ID和升级点数，根据这个升级角色的信息
	UFUNCTION(BlueprintCallable)
	FCharacterSavedInfo UpgradeCharacter(int32 ID, int32 UpgradePoints);
	// 得到用户拥有的所有角色的信息
	UFUNCTION(BlueprintCallable)
	TArray<UCharacterEntryItem*> GetAllOwnedCharacters();
	UFUNCTION(BlueprintGetter)
	const FTurrentAdditionalInfo GetCharacterAdditionalInfo(int32 ID);
	UFUNCTION(BlueprintGetter)
	const FEntityParams GetEntityParam(int32 ID);
	// 增加一个角色
	UFUNCTION(BlueprintCallable)
	void AddNewCharacter(int32 ID);
private:
	UArchiveSystem* ArchiveSystem;
	TMap<int32, FCharacterRankConfig*> RankConfigs;
	TArray<FEntityParams> AllEntityParamArray;

	
	void LoadAllRankConfig();
};


#pragma once
#include "CharacterDatas.generated.h"

// 配置每个角色的升级策略，每次升级
USTRUCT(BlueprintType)
struct FCharacterRankConfig:public FTableRowBase
{
	GENERATED_BODY()
public:
	// 目标炮塔id
	UPROPERTY(EditAnywhere)
	int32 TurretID;
	// 初始角色升级点数
	UPROPERTY(EditAnywhere)
	int32 InitialUpgradePoints;
	// 每一个升级需要增长的升级点数
	UPROPERTY(EditAnywhere)
	int32 IncreaseEachLevelUpgradePoints;
	// 每升一级加的生命值 
	UPROPERTY(EditAnywhere)
	int32 MaxHpIncrease;
	// 每升一个等级加的防御值
	UPROPERTY(EditAnywhere)
	int32 DefenceIncrease;
	// 每升一个等级加的攻击力
	UPROPERTY(EditAnywhere)
	int32 AttackIncrease;
	// 最大等级
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	static const int32 MaxLevel = 70;
};

//每个角色保存的信息
USTRUCT(BlueprintType)
struct FCharacterSavedInfo
{
	GENERATED_BODY()
	// 目标炮塔的id
	UPROPERTY()
	int32 TargetTurretId;
	// 当前等级
	UPROPERTY()
	int32 CurrentLevel;
	// 需要升级到下一级所需要全部的点数
	UPROPERTY()
	int32 TotalUpgradeNextNeededPoints;
	// 剩余的升级点数
	UPROPERTY()
	int32 LeftUpgradePoints;
	// 当前的生命上限
	UPROPERTY()
	int32 MaxHP;
	// 当前的防御值
	UPROPERTY()
	int32 Defence;
	// 攻击值
	UPROPERTY()
	int32 Attack;
	// 当前拿的武器guid
	UPROPERTY()
	FGuid AttachedWeaponID;
	// 圣遗物再说吧
};
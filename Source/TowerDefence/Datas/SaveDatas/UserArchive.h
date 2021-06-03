#pragma once
#include "GameFramework/SaveGame.h"
#include "TowerDefence/Datas/CharacterDatas.h"

#include "UserArchive.generated.h"

// 玩家的背包内容
//USTRUCT()
// struct FUserPackage
// {
// 	GENERATED_BODY()
// 	// 食物栏
// 	UPROPERTY()
// 	// TMap<FGuid, > Foods;
// 	// // 武器栏
// 	// UPROPERTY()
// 	// TArray<FGuid, >
// 	
// };
// 用户相关存档
UCLASS()
class UUserArchive: public USaveGame
{
	GENERATED_BODY()
public:

	UUserArchive(const FObjectInitializer& ObjectInitializer);
	// 体力上限
	// 游戏内金币数量
	UPROPERTY(BlueprintReadOnly)
	int32 GameCoinNum;
	// 付费充值的金币数量
	UPROPERTY(BlueprintReadOnly)
	int32 PaidCoinNum;
	// 该用户拥有的所有角色信息
	UPROPERTY(BlueprintReadOnly)
	TMap<int32, FCharacterSavedInfo> OwnedCharacters;
	UPROPERTY(BlueprintReadOnly)
	bool IsFirstStartGame = false;
	// 上一次添加体力值的时间
	UPROPERTY(BlueprintReadOnly)
	FDateTime LastAddPhysicalPointTime;
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentPhysicalStrength();
	UFUNCTION(BlueprintCallable)
	int32 GetMaxPhysicalStrength();
	UFUNCTION(BlueprintCallable)
	bool UsePhysicalStrength(int32 InPoints);
protected:
	// 当前体力
	UPROPERTY(BlueprintReadOnly)
	int32 PhysicalStrength;
};

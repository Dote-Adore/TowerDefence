#pragma once
#include "GameFramework/SaveGame.h"
#include "PackageArchive.generated.h"


USTRUCT(BlueprintType)
struct FWeaponItemArchive
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	int32 TargetWeaponID;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	int32 Level;
};


// 存放用户的背包内容
UCLASS()
class UPackageArchive:public USaveGame
{
	GENERATED_BODY()
public:
	void DeleteFood(int32 FoodID,int32 Num);
	void AddFood(int32 FoodID, int32 Num);
	void AddDevelopment(int32 DevelopID, int32 Num);
	void DeleteDevelopment(int32 DevelopID, int32 Num);
	// 武器
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<FGuid, FWeaponItemArchive> Weapons;
	// 食物id，和拥有数量
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<int32, int32> Foods;
	// 升级素材，物品id和拥有数量
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<int32, int32> Developments;
	
};
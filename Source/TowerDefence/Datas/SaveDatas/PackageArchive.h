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

USTRUCT(BlueprintType)
struct FFoodItemArchive
{
	GENERATED_BODY()
	// 键代表FoodId,值代表拥有的数量
	TMap<int32, int32> Foods;
};

// 存放用户的背包内容
UCLASS()
class UPackageArchive:public USaveGame
{
	GENERATED_BODY()
public:
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
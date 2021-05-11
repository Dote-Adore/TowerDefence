#pragma once
#include "Engine/DataTable.h"
#include "PackageItem.generated.h"

// Item获取途径
UCLASS(BlueprintType)
class UItemGainedMethod:public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Intro;
	UFUNCTION(BlueprintCallable)
	// 导航
	virtual void OnGuide(){};
};

UENUM()
enum class ERareness
{
	OneStar,
	TwoStar,
	ThreeStar,
	fourStar,
	FiveStar
};

USTRUCT(BlueprintType)
struct FBaseItem:public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> ItemImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Introduction;
	// 稀有度，一星到五星
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERareness Rareness;
	// 获取来源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UItemGainedMethod>> GainedSources;
};


// 武器类
USTRUCT(BlueprintType)
struct FWeaponItem:public FBaseItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> Model;
	// 基础攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseAttackValue;
	// 额外数值更改
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, float> AdditionalParams;
};
UENUM()
enum class EFoodType
{
	// 对某个特定角色使用
	Single,
	// 对所有角色使用
	All
};
// 食物类,使用后相当于使用一个buff
USTRUCT(BlueprintType)
struct FFoodItem:public FBaseItem
{
	// 使用后的BuffID
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UsedBuffID;
	// 使用对象，单个目标还是全部角色加成
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EFoodType Type = EFoodType::Single;
};

USTRUCT(BlueprintType)
struct FCharacterDevelopmentItem:public FBaseItem
{
	GENERATED_BODY()
};
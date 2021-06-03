#pragma once
#include "TowerDefence/Datas/PackageItem.h"
#include "PackageSystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PackageSystem, Log, All);


USTRUCT(BlueprintType)
struct FFoodItemEntry
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FFoodItem Item;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Num;
	FFoodItemEntry()
		:Item(FFoodItem())
	{
	}
	FFoodItemEntry(FFoodItem& InItem, int32 InNum)
		:Item(InItem), Num(InNum)
	{}
};

USTRUCT(BlueprintType)
struct FCharacterDevelopmentItemEntry
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FCharacterDevelopmentItem Item;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Num;
	FCharacterDevelopmentItemEntry(FCharacterDevelopmentItem& InItem, int32 InNum)
		:Item(InItem), Num(InNum)
	{}
	FCharacterDevelopmentItemEntry()
		:Item(FCharacterDevelopmentItem())
	{}
};
class UArchiveSystem;
UCLASS()
class TOWERDEFENCE_API UPackageSystem:public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UFUNCTION(BlueprintCallable)
	TArray<FFoodItemEntry> GetAllFoods();
	UFUNCTION(BlueprintCallable)
	TArray<FCharacterDevelopmentItemEntry> GetAllDevelopments();
	UFUNCTION(BlueprintCallable)
	FFoodItemEntry GetOneFood(int32 ID);
	UFUNCTION(BlueprintCallable)
	FCharacterDevelopmentItemEntry GetOneCharacterDevelopment(int32 ID);
	UFUNCTION(BlueprintCallable)
	void DeleteFoodItem(int32 ID, int32 Num);
	UFUNCTION(BlueprintCallable)
	void DelteDevelopmentItem(int32 ID, int32 Num);
	UFUNCTION(BlueprintCallable)
	void AddFoodItem(int32 ID, int32 Num);
	UFUNCTION(BlueprintCallable)
	void AddDevelopItem(int32 ID, int32 Num);
private:

	void LoadAllPackageItemConfig();
	UPROPERTY(Transient)
	UArchiveSystem* ArchiveSystem;
	UPROPERTY(Transient)
	TMap<int32, FFoodItem> AllFoodsConfig;
	UPROPERTY(Transient)
	TMap<int32, FCharacterDevelopmentItem> AllDevelopmentItemConfig;
};
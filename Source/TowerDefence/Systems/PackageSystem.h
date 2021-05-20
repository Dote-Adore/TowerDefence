#pragma once
#include "TowerDefence/Datas/CharacterDatas.h"
#include "TowerDefence/Datas/PackageItem.h"
#include "TowerDefence/Datas/SaveDatas/PackageArchive.h"

#include "PackageSystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PackageSystem, Log, All);


USTRUCT(BlueprintType)
struct FFoodItemEntry
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FFoodItem& Item;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Num;
	FFoodItemEntry(FFoodItem& InItem, int32 InNum)
		:Item(InItem), Num(InNum)
	{}
};

USTRUCT(BlueprintType)
struct FCharacterDevelopmentItemEntry
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FCharacterDevelopmentItem& Item;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Num;
	FCharacterDevelopmentItemEntry(FCharacterDevelopmentItem& InItem, int32 InNum)
		:Item(InItem), Num(InNum)
	{}
};

class UArchiveSystem;
UCLASS()
class UPackageSystem:public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	TArray<FFoodItemEntry> GetAllFoods();
	TArray<FCharacterDevelopmentItemEntry> GetAllDevelopments();
	FFoodItemEntry GetOneFood(int32 ID);
	FCharacterDevelopmentItemEntry GetOnCharacterDevelopment(int32 ID);
	
	void DeleteFoodItem(int32 ID, int32 Num);
	void DelteDevelopmentItem(int32 ID, int32 Num);
private:

	void LoadAllPackageItemConfig();
	
	UArchiveSystem* ArchiveSystem;
	UPROPERTY(Transient)
	TMap<int32, FFoodItem> AllFoodsConfig;
	UPROPERTY(Transient)
	TMap<int32, FCharacterDevelopmentItem> AllDevelopmentItemConfig;
};
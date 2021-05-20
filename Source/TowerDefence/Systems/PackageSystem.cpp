#include "PackageSystem.h"

#include "ArchiveSystem.h"
#include "TowerDefence/GlobalConfig.h"

DEFINE_LOG_CATEGORY(PackageSystem)

void UPackageSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
	LoadAllPackageItemConfig();
}

TArray<FFoodItemEntry> UPackageSystem::GetAllFoods()
{
	TArray<FFoodItemEntry> Res;
	for(auto Food: ArchiveSystem->GetPackageArchive()->Foods)
	{
		FFoodItemEntry ItemEntry(*AllFoodsConfig.Find(Food.Key), Food.Value);
		Res.Add(ItemEntry);
	}
	return Res;
}

TArray<FCharacterDevelopmentItemEntry> UPackageSystem::GetAllDevelopments()
{
	TArray<FCharacterDevelopmentItemEntry> Res;
	for(auto Development: ArchiveSystem->GetPackageArchive()->Developments)
	{
		FCharacterDevelopmentItemEntry ItemEntry(*AllDevelopmentItemConfig.Find(Development.Key), Development.Value);
		Res.Add(ItemEntry);
	}
	return Res;
}

FFoodItemEntry UPackageSystem::GetOneFood(int32 ID)
{
	int32 Num = 0;
	for(auto Food: ArchiveSystem->GetPackageArchive()->Foods)
	{
		if(Food.Key != ID)
			continue;
		Num = Food.Value;
	}
	FFoodItem* Item = AllFoodsConfig.Find(ID);
	FFoodItemEntry Res(*Item, Num);
	return Res;

}

FCharacterDevelopmentItemEntry UPackageSystem::GetOnCharacterDevelopment(int32 ID)
{
	int32 Num = 0;
	for(auto Development: ArchiveSystem->GetPackageArchive()->Developments)
	{
		if(Development.Key != ID)
			continue;
		Num = Development.Value;
	}
	FCharacterDevelopmentItem* Item = AllDevelopmentItemConfig.Find(ID);
	FCharacterDevelopmentItemEntry Res(*Item, Num);
	return Res;
}

void UPackageSystem::DeleteFoodItem(int32 ID, int32 Num)
{
	ArchiveSystem->GetPackageArchive()->DeleteFood(ID,Num);
	ArchiveSystem->SaveArchive();
}

void UPackageSystem::DelteDevelopmentItem(int32 ID, int32 Num)
{
	ArchiveSystem->GetPackageArchive()->DeleteDevelopment(ID,Num);
	ArchiveSystem->SaveArchive();
}

void UPackageSystem::LoadAllPackageItemConfig()
{
	AllFoodsConfig.Empty();
	const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
	UDataTable* FoodItems =  Config->FoodItems.LoadSynchronous();
	if(!FoodItems)
	{
		UE_LOG(PackageSystem, Error, TEXT("Can not find FoodItems DataTable in GlobalConfig!"));
		return;
	}
	TArray<FFoodItem*> FoodItemArray;
	FoodItems->GetAllRows<FFoodItem>(GET_MEMBER_NAME_STRING_CHECKED(FFoodItem, ID), FoodItemArray);
	for(FFoodItem* FoodItem:FoodItemArray)
	{
		AllFoodsConfig.Add(FoodItem->ID, *FoodItem);
	}

	// development
	UDataTable* DevelopmentTable = Config->DevelopmentItems.LoadSynchronous();
	if(!DevelopmentTable)
	{
		UE_LOG(PackageSystem, Error, TEXT("Can not find DevelopmentItem DataTable in GlobalConfig!"));
		return;
	}
	TArray<FCharacterDevelopmentItem*> DevelopmentArray;
	DevelopmentTable->GetAllRows<FCharacterDevelopmentItem>(
	GET_MEMBER_NAME_STRING_CHECKED(FCharacterDevelopmentItem, ID),DevelopmentArray);
	for(FCharacterDevelopmentItem* Item: DevelopmentArray)
	{
		AllDevelopmentItemConfig.Add(Item->ID, *Item);
	}
}

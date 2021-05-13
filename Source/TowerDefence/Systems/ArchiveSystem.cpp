#include "ArchiveSystem.h"
#include "TowerDefence/Datas/SaveDatas/LevelArchive.h"
#include "TowerDefence/Datas/SaveDatas/UserArchive.h"
#include "Async/Async.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Datas/SaveDatas/PackageArchive.h"

void UArchiveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// 在这里添加存档
	LoadArchive<UUserArchive>(SavedUserArchive, TEXT("UserArchive"));
	LoadArchive<ULevelArchive>(SavedLevelArchive, TEXT("LevelArchive"));
	LoadArchive<UPackageArchive>(SavedPackageArchive, TEXT("PackageArchive"));
	UE_LOG(LogTemp, Display, TEXT("Initialze ArchiveSystem Success!"))
}

void UArchiveSystem::SaveArchive()
{
	SaveArchive(FOnSaveArchiveDoneDelegate());
}

void UArchiveSystem::SaveArchive(FOnSaveArchiveDoneDelegate SavedDelegate)
{
	bool bRet = true;
	for(auto ArchiveName: ArchiveNames)
	{
		bRet = bRet & UGameplayStatics::SaveGameToSlot(SavedUserArchive, ArchiveName,0);
	}
	if(SavedDelegate.IsBound())
	{
		SavedDelegate.ExecuteIfBound(bRet);
	}
}

UUserArchive* UArchiveSystem::GetUserArchive()
{
	return SavedUserArchive;
}

ULevelArchive* UArchiveSystem::GetLevelArchive()
{
	return SavedLevelArchive;
}

UPackageArchive* UArchiveSystem::GetPackageArchive()
{
	return SavedPackageArchive;
}

void UArchiveSystem::Deinitialize()
{
	Super::Deinitialize();
}

template <class SavedClass>
void UArchiveSystem::LoadArchive(SavedClass*& outVal, const FString& SlotName)
{
	bool bRet = UGameplayStatics::DoesSaveGameExist(SlotName, 0);
	
	if(bRet)
	{
		UE_LOG(LogTemp, Display, TEXT("Load Archive '%s', success!"), *SlotName);
		ArchiveNames.Add(SlotName);
		outVal = Cast<SavedClass>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Load Archive '%s', failed!"), *SlotName);

		outVal = Cast<SavedClass>(UGameplayStatics::CreateSaveGameObject(SavedClass::StaticClass()));
	}
}

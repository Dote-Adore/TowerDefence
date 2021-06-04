﻿#include "ArchiveSystem.h"
#include "TowerDefence/Datas/SaveDatas/LevelTaskArchive.h"
#include "TowerDefence/Datas/SaveDatas/UserArchive.h"
#include "Async/Async.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Datas/SaveDatas/PackageArchive.h"

void UArchiveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// 在这里添加存档
	LoadArchive();
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
		bRet = bRet & UGameplayStatics::SaveGameToSlot(ArchiveName.Value, ArchiveName.Key,0);
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

ULevelTaskArchive* UArchiveSystem::GetLevelArchive()
{
	return SavedLevelTaskArchive;
}

UPackageArchive* UArchiveSystem::GetPackageArchive()
{
	return SavedPackageArchive;
}

void UArchiveSystem::Deinitialize()
{
	Super::Deinitialize();
}

void UArchiveSystem::ClearArchive()
{
	for(auto ArchiveName: ArchiveNames)
	{
		UGameplayStatics::DeleteGameInSlot(ArchiveName.Key, 0);
	}
	LoadArchive();
}

void UArchiveSystem::LoadArchive()
{
	_LoadArchive<UUserArchive>(SavedUserArchive, TEXT("UserArchive"));
	_LoadArchive<ULevelTaskArchive>(SavedLevelTaskArchive, TEXT("LevelArchive"));
	_LoadArchive<UPackageArchive>(SavedPackageArchive, TEXT("PackageArchive"));
	UE_LOG(LogTemp, Display, TEXT("Initialze ArchiveSystem Success!"))
}

template <class SavedClass>
void UArchiveSystem::_LoadArchive(SavedClass*& outVal, const FString& SlotName)
{
	bool bRet = UGameplayStatics::DoesSaveGameExist(SlotName, 0);
	
	if(bRet)
	{
		UE_LOG(LogTemp, Display, TEXT("Load Archive '%s', success!"), *SlotName);
		outVal = Cast<SavedClass>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		ArchiveNames.Add(SlotName, outVal);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Load Archive '%s', failed!"), *SlotName);

		outVal = Cast<SavedClass>(UGameplayStatics::CreateSaveGameObject(SavedClass::StaticClass()));
		ArchiveNames.Add(SlotName, outVal);
	}
}

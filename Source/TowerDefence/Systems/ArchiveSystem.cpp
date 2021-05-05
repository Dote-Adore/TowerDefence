#include "ArchiveSystem.h"
#include "TowerDefence/Datas/SaveDatas/LevelArchive.h"
#include "TowerDefence/Datas/SaveDatas/UserArchive.h"
#include "Async/Async.h"
#include "Kismet/GameplayStatics.h"

void UArchiveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadArchive<UUserArchive>(SavedUserArchive, TEXT("UserArchive"));
	LoadArchive<ULevelArchive>(SavedLevelArchive, TEXT("LevelArchive"));
}

void UArchiveSystem::SaveArchive()
{
	SaveArchive(FOnSaveArchiveDoneDelegate());
}

void UArchiveSystem::SaveArchive(FOnSaveArchiveDoneDelegate SavedDelegate)
{
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this,SavedDelegate]()
	{
		bool bRet = true;
		bRet = bRet & UGameplayStatics::SaveGameToSlot(SavedUserArchive, TEXT("UserArchive"),0);
		bRet = bRet & UGameplayStatics::SaveGameToSlot(SavedLevelArchive, TEXT("LevelArchive"), 0);
		if(SavedDelegate.IsBound())
		{
			AsyncTask(ENamedThreads::GameThread, [SavedDelegate,bRet]()
			{
				SavedDelegate.ExecuteIfBound(bRet);
			});
		}
	});
}

UUserArchive* UArchiveSystem::GetUserArchive()
{
	return SavedUserArchive;
}

ULevelArchive* UArchiveSystem::GetLevelArchive()
{
	return SavedLevelArchive;
}

void UArchiveSystem::Deinitialize()
{
	Super::Deinitialize();
	// SaveArchive();

}

template <class SavedClass>
void UArchiveSystem::LoadArchive(SavedClass*& outVal, const FString& SlotName)
{
	bool bRet = UGameplayStatics::DoesSaveGameExist(SlotName, 0);
	
	if(bRet)
	{
		UE_LOG(LogTemp, Display, TEXT("Load Archive '%s', success!"), *SlotName);
		outVal = Cast<SavedClass>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Load Archive '%s', failed!"), *SlotName);

		outVal = Cast<SavedClass>(UGameplayStatics::CreateSaveGameObject(SavedClass::StaticClass()));
	}
}

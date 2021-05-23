// 存档系统，专门处理存档功能
#pragma once
#include "GameFramework/SaveGame.h"
#include "ArchiveSystem.generated.h"

class UUserArchive;
class ULevelTaskArchive;
class UPackageArchive;
// 参数：是否保存成功
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSaveArchiveDoneDelegate, bool, bSuccess);
UCLASS()
class UArchiveSystem:public UGameInstanceSubsystem 
{
	GENERATED_BODY()
	// GENERATED_SINGLETON_BODY(UArchiveSystem)
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// 存档
	void SaveArchive();
	UFUNCTION(BlueprintCallable)
	void SaveArchive(FOnSaveArchiveDoneDelegate OnSaveArchiveDone);
	// 获取玩家存档
	UFUNCTION(BlueprintCallable)
	UUserArchive* GetUserArchive();
	UFUNCTION(BlueprintCallable)
	ULevelTaskArchive* GetLevelArchive();
	UFUNCTION(BlueprintCallable)
	UPackageArchive* GetPackageArchive();
	virtual void Deinitialize() override;
	

private:
	// 读档
	template<class SavedClass>
	void LoadArchive(SavedClass*& outVal, const FString& SlotName);


	UPROPERTY(Transient)
	UUserArchive* SavedUserArchive;
	UPROPERTY(Transient)
	ULevelTaskArchive* SavedLevelTaskArchive;
	UPROPERTY(Transient)
	UPackageArchive* SavedPackageArchive;
	UPROPERTY(Transient)
	TMap<FString, USaveGame*> ArchiveNames;
};
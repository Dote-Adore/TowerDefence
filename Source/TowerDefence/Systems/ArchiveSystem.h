// 存档系统，专门处理存档功能
#pragma once
#include "ArchiveSystem.generated.h"

class UUserArchive;
class ULevelArchive;
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
	ULevelArchive* GetLevelArchive();
	virtual void Deinitialize() override;

private:
	// 读档
	template<class SavedClass>
	void LoadArchive(SavedClass*& outVal, const FString& SlotName);


private:
	UUserArchive* SavedUserArchive;
	ULevelArchive* SavedLevelArchive;
};
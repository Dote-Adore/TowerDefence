#pragma once
#include "TowerDefence/Datas/CharacterDatas.h"

#include "PackageSystem.generated.h"

class UArchiveSystem;
UCLASS()
class UPackageSystem:public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void Re
private:
	// 使用物品
	void UseItem(int32 ItemType)
	UPROPERTY(Transient)
	UArchiveSystem* ArchiveSystem;
};
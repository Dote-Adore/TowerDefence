#pragma once
#include "TowerDefence/Entities/Base/Entity.h"
#include "EntityCreator.generated.h"

UCLASS()
class UEntityCreator:public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void CreateTurrent(int32 EntityID, FTransform EntityTransform);
    UFUNCTION(BlueprintCallable)
    void CreateEnemy(int32 EntityID, FTransform EntityTransform);
private:
    void CreateEntity(int32 EntityID, FTransform EntityTransform, TSubclassOf<AEntity> Entitylass, UDataTable* EntityDatas, UDataTable* AnimDatas);  
};
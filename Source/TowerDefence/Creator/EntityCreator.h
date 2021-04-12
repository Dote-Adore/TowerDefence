#pragma once
#include "EntityCreator.generated.h"


class AEntity;
class ATurrent;
class AEnemy;
class UDataTable;
UCLASS(Blueprintable)
class UEntityCreator:public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    ATurrent* CreateTurrent(int32 EntityID, FTransform EntityTransform);
    UFUNCTION(BlueprintCallable)
    AEnemy* CreateEnemy(int32 EntityID, FTransform EntityTransform);
private:
    AEntity* CreateEntity(int32 EntityID, FTransform EntityTransform, TSubclassOf<AEntity> Entitylass, UDataTable* EntityDatas, UDataTable* AnimDatas);  
};
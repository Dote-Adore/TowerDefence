#pragma once
#include "EnemyMovementComponent.generated.h"


class AEnemy;
class ABaseTile;
UCLASS()
class UEnemyMovementComponent:public UActorComponent
{
	GENERATED_BODY()
public:
	void SetPath(TArray<const ABaseTile*> TilesPath);
	UEnemyMovementComponent(const FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AEnemy* ParentEnemy;
	TArray<const ABaseTile*> EnemyWalkTiles;
	int32 CurrentWalkToTileIdx = 0;
};

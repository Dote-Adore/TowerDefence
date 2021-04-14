#pragma once
#include "EnemyMovementComponent.generated.h"



class AEnemy;
class ABaseTile;
DECLARE_EVENT_OneParam(UEnemyMovementComponent, FOnEnemyArrivalToEndDelegate, AEnemy*);
UCLASS()
class UEnemyMovementComponent:public UActorComponent
{
	GENERATED_BODY()
public:
	void SetPath(TArray<const ABaseTile*> TilesPath);
	UEnemyMovementComponent(const FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static FOnEnemyArrivalToEndDelegate OnEnemyArrivalToEndDelegate;
private:
	AEnemy* ParentEnemy;
	TArray<const ABaseTile*> EnemyWalkTiles;
	int32 CurrentWalkToTileIdx = 0;
	bool HasBoardCastOnArrivalEndEvent;
};

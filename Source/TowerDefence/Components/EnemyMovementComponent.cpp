#include "EnemyMovementComponent.h"
#include "TowerDefence/Entities/Enemy.h"
#include "TowerDefence/Entities/Base/Tile.h"

FOnEnemyArrivalToEndDelegate UEnemyMovementComponent::OnEnemyArrivalToEndDelegate = FOnEnemyArrivalToEndDelegate();

UEnemyMovementComponent::UEnemyMovementComponent(const FObjectInitializer& ObjectInitializer)
	:UActorComponent(ObjectInitializer)
{
	HasBoardCastOnArrivalEndEvent = false;
	PrimaryComponentTick.bCanEverTick = true;
	ParentEnemy = Cast<AEnemy>(GetOwner());
}

void UEnemyMovementComponent::SetPath(TArray<const ABaseTile*> TilesPath)
{
	EnemyWalkTiles = TilesPath;
	ParentEnemy->SetActorLocation(EnemyWalkTiles[0]->GetSpawnEntityLocation());
}

void UEnemyMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(EnemyWalkTiles.Num() == 0)
		return;

	// 结束了，走到了最后
	if(CurrentWalkToTileIdx == EnemyWalkTiles.Num() -1)
	{
		if(HasBoardCastOnArrivalEndEvent == false)
		{
			OnEnemyArrivalToEndDelegate.Broadcast(ParentEnemy);
			HasBoardCastOnArrivalEndEvent = true;
		}
		return;
	}

	
	int32 TargetMoveToTileIdx = CurrentWalkToTileIdx + 1;
	const ABaseTile* TargetMoveToTile = EnemyWalkTiles[TargetMoveToTileIdx];
	// 如果目标位置已经到了，则切换移动的目标
	FVector TargetMoveToLocation = TargetMoveToTile->GetSpawnEntityLocation();
	if(FMath::IsNearlyEqual(TargetMoveToLocation.X, ParentEnemy->GetActorLocation().X, 1)&&
		FMath::IsNearlyEqual(TargetMoveToLocation.Y, ParentEnemy->GetActorLocation().Y, 1))
	{
		CurrentWalkToTileIdx++;
		return;
	}
	// 如果当前有攻击对象，则停止walk
	if (ParentEnemy->CurrentAttackedEntities.Num()>0||ParentEnemy->GetCurrentEntityParams().CurrentHP<=0)
	{
		return;
	}
	FVector Direction = TargetMoveToLocation - ParentEnemy->GetActorLocation();
	Direction = Direction.GetSafeNormal2D();
	ParentEnemy->AddMovementInput(Direction, ParentEnemy->GetCurrentEntityParams().AttackSpeedRate);
}

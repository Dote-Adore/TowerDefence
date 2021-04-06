#pragma once
#include "Tile.generated.h"

class UBoxComponent;
class AEntity;
UENUM()
enum ETileType
{
	Start,
	End,
	Disable,
	// 敌人可以走的道路
	Path,
	// 可部署的
	Deployable
};

UCLASS(BlueprintType)
class ABaseTile:public  AActor
{
	GENERATED_BODY()
public:
	ABaseTile(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(EditDefaultsOnly)
	ETileType TileType;
	UPROPERTY(EditDefaultsOnly)
	FName DeployableEntityType;

	void SetDeployEntity(AEntity* TargetEntity);
	bool CanDeploy();
protected:
	// 当前部署在上面的实体
	UPROPERTY(Transient)
	AEntity* CurrentDeployEntity;
	UPROPERTY()
	UBoxComponent* BoxComponent;
	UPROPERTY()
	float BoxSize = 200.f;
};

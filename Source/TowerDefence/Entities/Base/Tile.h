#pragma once
#include "Tile.generated.h"

class UBoxComponent;
class AEntity;
UENUM()
enum class ETileType
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
	//可部署的EntityTypes
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> DeployableEntityTypes;
#if WITH_EDITOR
	UPROPERTY(EditDefaultsOnly)
	FLinearColor DebugColor;
#endif


	void SetDeployEntity(AEntity* TargetEntity);
	bool CanDeploy();
	void Deploy(AEntity* Entity);
protected:
	// 当前部署在上面的实体
	UPROPERTY(Transient)
	AEntity* CurrentDeployEntity;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;
	UPROPERTY()
	float BoxSize = 200.f;
	UPROPERTY(EditAnywhere)
	float HeightOffest = 0.f;
};

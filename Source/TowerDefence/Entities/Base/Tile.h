#pragma once
#include "Tile.generated.h"

class UBoxComponent;
class AEntity;
class ABaseTile;
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
class TOWERDEFENCE_API ABaseTile:public  AActor
{
	GENERATED_BODY()
public:
	ABaseTile(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(EditDefaultsOnly)
	ETileType TileType;
	//可部署的EntityTypes
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> DeployableEntityTypes;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor DebugColor;
	static const float BoxSize;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void OnConstruction(const FTransform& Transform) override;
	void SetDeployEntity(AEntity* TargetEntity);
	bool CanDeploy(FName TargetCategory);
	void Deploy(AEntity* Entity);
	void ChangePlaneColor(FLinearColor InColor);
	FVector GetSpawnEntityLocation() const;
protected:
	// 当前部署在上面的实体
	UPROPERTY(Transient)
	AEntity* CurrentDeployEntity;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SelectedPlaneComponent;
	UPROPERTY(EditAnywhere)
	float HeightOffest = 0.f;

	UMaterialInstanceDynamic* SlectedShowMID;
};

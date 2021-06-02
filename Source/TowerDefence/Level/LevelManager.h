#pragma once
#include "TowerDefence/TDPlayerController.h"
#include "TowerDefence/Systems/LevelTaskSystem.h"
#include "LevelManager.generated.h"


class ABaseTile;
struct FEnemyGenerationInfo;
class UStateMachineComponent;
class UTDGameInstance;
class UEntityCreator;
class ALevelManager;
class AEnemy;
class ATurrent;
// DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnTileClickedSignature, ALevelManager, OnTileClicked, ABaseTile*, TargetTile);
// 闯关失败事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelFailedDelegate);
// 闯关成功事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelSuccessDelegate, FFinishReward, Reward);
// 下一波事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextWaveDeleagate);
// 部署成功事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeployTurrentEvent, ATurrent*, Turrent, ABaseTile*, TargetTile);
UCLASS()
class ALevelManager:public AActor
{
	friend class UBaseWaveState;
	GENERATED_BODY()
public:
	
	ALevelManager(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	TSharedPtr<const FEnemyGenerationInfo> GetCurrentWaveInfoPtr();
	TArray<ABaseTile*> GetAllTiles() {return AllTiles; };

	void OnLevelSuccess();
	UFUNCTION(BlueprintCallable)
	void  OnRequestToDeploy(int32 TurrentID, FName Category, int32 Cost);
	UFUNCTION(BlueprintCallable)
	void OnCancelDeploy();

	void OnNextWave();
	UPROPERTY()
	UEntityCreator* EntityCreator;
	// 该关卡的部署点数
	UPROPERTY(BlueprintReadWrite)
	int32 DeployPoint;

	UPROPERTY(BlueprintAssignable)
	FOnLevelFailedDelegate OnLevelFailedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnLevelSuccessDelegate OnLevelSuccessDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnNextWaveDeleagate OnNextWaveDeleagate;
	UPROPERTY(BlueprintAssignable)
	FOnDeployTurrentEvent OnDeployTurrentEvent;
	
	// ----wave state params----
	// 当前波数敌人死亡的数量
	int32 CurrentWaveEnemyDeathNum;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentWaveIdx;
	UPROPERTY(BlueprintReadOnly)
	int32 TotalWaves;
	UPROPERTY(BlueprintReadOnly)
	int32 SurplusEnemyArrivalNum;
	// -------------------------


private:
	FTimerHandle StartGameDelayTimeHandle;
	UStateMachineComponent* StateMachineComponent;
	TArray<ABaseTile*> AllTiles;
	FLevelTaskItem CurrentLevelTaskItem;
	void GenerateLevelMap();

	
	// ---------------  Some Deploy Params
	FLinearColor CanDeployColor = FColor(86, 186, 38 , 125);
	int32 TargetGeneratedID;
	FName TargetGeneratedCategory;
	int32 TargetDeployCost;
	// UFUNCTION()
	void OnTileClickedListener(ABaseTile* TargetTile);
	// ------------------


	// 敌人死亡的监听事件
	void OnEnemyDeathListener(AEnemy* TargetDeathEnemy);
	void OnEnemyArrivalToTheEndListener(AEnemy* TargetDeathEnemy);
};

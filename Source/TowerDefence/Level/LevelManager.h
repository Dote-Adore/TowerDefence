#pragma once
#include "TowerDefence/TDPlayerController.h"
#include "LevelManager.generated.h"

class ABaseTile;
struct FEnemyGenerationInfo;
class UStateMachineComponent;
class UTDGameInstance;
class UEntityCreator;
class ALevelManager;
class AEnemy;
// DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnTileClickedSignature, ALevelManager, OnTileClicked, ABaseTile*, TargetTile);
UCLASS()
class ALevelManager:public AActor
{
	friend class UBaseWaveState;
	enum ERoundState
	{
		// 初始化
		Init,
		// 生成敌人
		GenerateEnemies,
		// 等待所有敌人死亡，到下一个回合
		WaitForNext,
		// 结束
		End
	};
	GENERATED_BODY()
public:
	
	ALevelManager(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	TSharedPtr<const FEnemyGenerationInfo> GetCurrentWaveInfoPtr();
	TArray<ABaseTile*> GetAllTiles() {return AllTiles; };

	void OnLevelPass();
	UFUNCTION(BlueprintCallable)
	void  OnRequestToDeploy(int32 TurrentID, FName Category, int32 Cost);
	UFUNCTION(BlueprintCallable)
	void OnCancelDeploy();
	UPROPERTY()
	UEntityCreator* EntityCreator;
	// 该关卡的部署点数
	UPROPERTY(BlueprintReadWrite)
	int32 DeployPoint;

	
	// ----wave state params----
	// 当前波数敌人死亡的数量
	int32 CurrentWaveEnemyDeathNum;
	int32 CurrentWaveIdx;
	int32 TotalWaves;
	// -------------------------


private:
	UStateMachineComponent* StateMachineComponent;
	UTDGameInstance* TDGameInstance;
	TArray<ABaseTile*> AllTiles;
	void GenerateLevelMap();
	ERoundState RoundState;
	// ---------------

	// Some Deploy Params
	FLinearColor CanDeployColor = FColor(86, 186, 38 , 125);
	int32 TargetGeneratedID;
	FName TargetGeneratedCategory;
	int32 TargetDeployCost;
	// UFUNCTION()
	void OnTileClickedListener(ABaseTile* TargetTile);
	// ------------------


	// 敌人死亡的监听事件
	void OnEnemyDeathListener(AEnemy* TargetDeathEnemy);
};

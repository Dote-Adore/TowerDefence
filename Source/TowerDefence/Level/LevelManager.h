#pragma once
#include "LevelManager.generated.h"

class ABaseTile;
struct FEnemyGenerationInfo;
class UStateMachineComponent;
class UTDGameInstance;
class UEntityCreator;
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
	UFUNCTION(BlueprintCallable)
	TArray<ABaseTile*> OnGetDeployableTiles(int32 TurrentID, FName Category);
	UFUNCTION(BlueprintCallable)
	void RequsetDeployToTile(int32 TurrentID, ABaseTile* TargetTile, int32 Cost);
	UFUNCTION(BlueprintCallable)
	void OnCancelDeploy();

	UEntityCreator* EntityCreator;
	// 该关卡的部署点数
	UPROPERTY(BlueprintReadWrite)
	int32 DeployPoint;
private:
	UStateMachineComponent* StateMachineComponent;
	UTDGameInstance* TDGameInstance;
	TArray<ABaseTile*> AllTiles;
	void GenerateLevelMap();
	ERoundState RoundState;
	int32 CurrentWaveIdx;
	int32 TotalWaves;

	int32 DeathEnemyNums = 0;
	int32 CurrentDeathTurrentNums = 0;



	// Some Deploy Params
	FLinearColor CanDeployColor = FColor(86, 186, 38 , 125);
	// ------------------
};

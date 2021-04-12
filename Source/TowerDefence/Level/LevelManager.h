#pragma once
#include "LevelManager.generated.h"

class ABaseTile;
struct FEnemyGenerationInfo;
class UStateMachineComponent;
class UTDGameInstance;

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
private:
	UStateMachineComponent* StateMachineComponent;
	UTDGameInstance* TDGameInstance;
	TArray<ABaseTile*> AllTiles;
	void GenerateLevelMap();
	// void StartRound(int32 RoundIdx);
	// void ShowEnemyPath(const FEnemyGenerationInfo& TargetWave);
	// void OnInitWave(float DeltaSeconds);
	// void OnGenerateWave(float DeltaSeconds);
	// void WaitForNextWave(float DeltaSeconds);
	// void OnEndWave(float DeltaSeconds);
	
	ERoundState RoundState;
	int32 CurrentWaveIdx;
	int32 TotalWaves;

	
	int32 DeathEnemyNums = 0;
	int32 CurrentDeathTurrentNums = 0;
};

#pragma once
#include "LevelManager.generated.h"

class UTDGameInstance;
UCLASS()
class ALevelManager:public AActor
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UTDGameInstance* TDGameInstance;

	void GenerateLevelMap();
};

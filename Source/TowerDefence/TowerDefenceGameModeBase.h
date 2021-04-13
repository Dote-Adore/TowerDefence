// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefenceGameModeBase.generated.h"

class ALevelManager;
class UTDGameInstance;
/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API ATowerDefenceGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	ALevelManager* GetLevelManager();
private:
	UTDGameInstance* GameInstance;
	ALevelManager* LevelManager;
};

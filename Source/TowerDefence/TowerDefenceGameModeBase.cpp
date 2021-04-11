// Copyright Epic Games, Inc. All Rights Reserved.


#include "TowerDefenceGameModeBase.h"


#include "TDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Level/LevelManager.h"

void ATowerDefenceGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ATowerDefenceGameModeBase::BeginPlay()
{
	UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0)->SetShowMouseCursor(true);
	GameInstance = Cast<UTDGameInstance>(GetGameInstance());
	check(GameInstance);
	LevelManager = GetWorld()->SpawnActor<ALevelManager>(ALevelManager::StaticClass());
}

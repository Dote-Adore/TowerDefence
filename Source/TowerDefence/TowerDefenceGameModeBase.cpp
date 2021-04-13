// Copyright Epic Games, Inc. All Rights Reserved.


#include "TowerDefenceGameModeBase.h"


#include "TDGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Level/LevelManager.h"

void ATowerDefenceGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	LevelManager = nullptr;
	Super::InitGame(MapName, Options, ErrorMessage);
	DefaultPawnClass = nullptr;
}

void ATowerDefenceGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0)->SetShowMouseCursor(true);
	GameInstance = Cast<UTDGameInstance>(GetGameInstance());
	check(GameInstance);
	LevelManager = GetWorld()->SpawnActor<ALevelManager>(ALevelManager::StaticClass());
	TSoftClassPtr<UUserWidget> MainUIClass(FSoftObjectPath(TEXT("WidgetBlueprintGeneratedClass'/Game/UI/Main/MainGameUI.MainGameUI_C'")));
	if(!MainUIClass.LoadSynchronous())
	{
		UE_LOG(LogTemp, Error, TEXT("Target Main UI Widget Load Failed!, please check '/Game/UI/Main/MainGameUI'"));
		return;
	}
	UUserWidget* Target = CreateWidget(GetWorld(), MainUIClass.LoadSynchronous());
	Target->AddToViewport();
}

ALevelManager* ATowerDefenceGameModeBase::GetLevelManager()
{
	return LevelManager;
}

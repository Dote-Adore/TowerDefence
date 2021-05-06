#include "TDGameInstance.h"


#include "Kismet/GameplayStatics.h"
#include "Level/LevelInfomation.h"

UTDGameInstance::UTDGameInstance(const FObjectInitializer& ObjectInitializer)
	:UGameInstance(ObjectInitializer)
{
}

void UTDGameInstance::OpenLevel(const ULevelInfomation* InLevelInfo)
{
	CurrentLevelInfomation = InLevelInfo;
	check(CurrentLevelInfomation);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), InLevelInfo->BackgroundWorld);
}

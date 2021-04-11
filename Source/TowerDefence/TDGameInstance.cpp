#include "TDGameInstance.h"


#include "Kismet/GameplayStatics.h"
#include "Level/LevelInfomation.h"

void UTDGameInstance::OpenLevel(const ULevelInfomation* InLevelInfo)
{
	CurrentLevelInfomation = InLevelInfo;
	check(CurrentLevelInfomation);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), InLevelInfo->BackgroundWorld);
}

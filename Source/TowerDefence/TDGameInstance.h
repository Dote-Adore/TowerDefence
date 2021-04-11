#pragma once
#include "Engine/GameInstance.h"
#include "TDGameInstance.generated.h"


class ULevelInfomation;
UCLASS(Config=Game)
class TOWERDEFENCE_API UTDGameInstance:public UGameInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	const ULevelInfomation* CurrentLevelInfomation;
	UFUNCTION(BlueprintCallable)
	void OpenLevel(const ULevelInfomation* InLevelInfo);
};

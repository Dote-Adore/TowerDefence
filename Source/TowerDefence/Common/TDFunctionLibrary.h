#pragma once
#include "CoreMinimal.h"
#include "TDFunctionLibrary.generated.h"

class ULevelInfomation;
UCLASS()
class TOWERDEFENCE_API UTDFunctionLibrary:public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static const TMap<FName, ULevelInfomation*> GetAllLevelInfomations();
};

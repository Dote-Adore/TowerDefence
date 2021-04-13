#pragma once
#include "CoreMinimal.h"
#include "TowerDefence/Entities/Base/Entity.h"
#include "TowerDefence/Datas/TurrentAdditional.h"
#include "TDFunctionLibrary.generated.h"

struct FTurrentAdditionalInfo;
class ULevelInfomation;
UCLASS()
class TOWERDEFENCE_API UTDFunctionLibrary:public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static const TMap<FName, ULevelInfomation*> GetAllLevelInfomations();
	UFUNCTION(BlueprintCallable)
	static TArray<FEntityParams> GetAllTurrentParams();
	UFUNCTION(BlueprintCallable)
	static FTurrentAdditionalInfo FindTurrentAdditionalInfoByID(int32 EntityID);
};

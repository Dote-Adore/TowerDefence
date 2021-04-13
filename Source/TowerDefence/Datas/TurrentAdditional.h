#pragma once
#include "Engine/DataTable.h"
#include "TurrentAdditional.generated.h"


UENUM()
enum class ETurrentRarityLevel
{
	TwoStar,
	ThreeStar,
	fourStar,
	FiveStar
};

// 配置entity 类型，这个将作为部署的时候对应可以部署的Tile中
USTRUCT(BlueprintType)
struct FTurrentAdditionalInfo: public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 EntityID;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Thumbnail;
	// 星级，2星到5星
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETurrentRarityLevel StarLevel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Introduction;
};

UCLASS(BlueprintType)
class UEntityParamsForWidgetEntity:public UObject
{
	GENERATED_BODY()
	
};
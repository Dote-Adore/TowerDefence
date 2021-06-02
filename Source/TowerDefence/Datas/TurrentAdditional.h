#pragma once
#include "Engine/DataTable.h"
#include "PackageItem.h"
#include "TurrentAdditional.generated.h"


// 配置entity 类型，这个将作为部署的时候对应可以部署的Tile中
USTRUCT(BlueprintType)
struct FTurrentAdditionalInfo: public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 EntityID;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Thumbnail;
	// 插画，在获得新角色的时候展示
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Illustration;
	// 星级，2星到5星
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERareness StarLevel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Introduction;
};

UCLASS(BlueprintType)
class UEntityParamsForWidgetEntity:public UObject
{
	GENERATED_BODY()
	
};
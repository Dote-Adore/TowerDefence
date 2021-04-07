#pragma once
#include "Engine/DataTable.h"
#include "EntityType.generated.h"


// 配置entity 类型，这个将作为部署的时候对应可以部署的Tile中
USTRUCT(BlueprintType)
struct FEntityType:public FTableRowBase
{
	GENERATED_BODY()

	// 类型名字
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	FName TypeName;
	// 对应类型ID
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TSet<int32> EntityIDs;
};

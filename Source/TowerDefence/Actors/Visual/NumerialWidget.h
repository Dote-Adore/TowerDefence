#pragma once
#include "Actor.h"
#include "NumerialWidget.generated.h"

UCLASS()
class ANumerialWidget:public AActor
{
	GENERATED_BODY()
public:
	ANumerialWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite)
	class UWidgetComponent* UI;
	// 存在时间
	UPROPERTY(EditDefaultsOnly)
	float ExsitTime = 3.f;

	
	void ShowUI(FVector worldLocation, int32 ShownValue, FLinearColor TextColor);
	UFUNCTION(BlueprintImplementableEvent)
	void OnShowUI(FVector worldLocation, int32 ShownValue, FLinearColor TextColor);
	void OnDestory();

private:
	// 销毁时间
	FTimerHandle DestoryTimerHandle;
	
};

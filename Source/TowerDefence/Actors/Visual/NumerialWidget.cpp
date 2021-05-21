#include "NumerialWidget.h"
#include "Components/WidgetComponent.h"
ANumerialWidget::ANumerialWidget(const FObjectInitializer& ObjectInitializer)
{
	UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI"));
	SetRootComponent(UI);
	UI->SetWidgetSpace(EWidgetSpace::Screen);
}

void ANumerialWidget::ShowUI(FVector worldLocation, int32 ShownValue, FLinearColor TextColor)
{
	FVector RandomLocationOffest = FVector(
		FMath::FRandRange(-10, 10),
		FMath::FRandRange(-10, 10),
		FMath::FRandRange(-10, 10));
	this->SetActorLocation(worldLocation+RandomLocationOffest);
	GetWorld()->GetTimerManager().SetTimer(DestoryTimerHandle, this, &ANumerialWidget::OnDestory, ExsitTime);
	OnShowUI(worldLocation, ShownValue, TextColor);
}

void ANumerialWidget::OnDestory()
{

	Destroy(true);
}

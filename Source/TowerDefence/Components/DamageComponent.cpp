#include "DamageComponent.h"

#include "TowerDefence/Actors/Visual/NumerialWidget.h"
#include "TowerDefence/Entities/Base/Entity.h"

void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	ParentEntity = Cast<AEntity>(GetOwner());
	if(ParentEntity == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Error To Load Damage Component: owner is not A AEntity!"));
		return;
	}
	ParentEntity->OnDamageDelegate.AddDynamic(this, &UDamageComponent::OnDamageEvent);

}

void UDamageComponent::OnDamageEvent(int32 DamageValue, FTransform DamageTransform)
{
	TSoftClassPtr<ANumerialWidget> NumerialWidgetActorClass(FSoftObjectPath(
TEXT("BlueprintGeneratedClass'/Game/Res/Blueprints/AboutBattle/BP_NumicalWidgetActor.BP_NumicalWidgetActor_C'")));
	if(!NumerialWidgetActorClass.LoadSynchronous())
	{
		UE_LOG(LogTemp, Error, TEXT("Can not found Target NumerialWidget In DamageBuff"));
		return;
	}
	ANumerialWidget* TargetWidget = ParentEntity->GetWorld()->SpawnActor<ANumerialWidget>(NumerialWidgetActorClass.LoadSynchronous());
	TargetWidget->ShowUI(DamageTransform.GetLocation(), DamageValue, FLinearColor(1,0.1,0.1, 1));
}


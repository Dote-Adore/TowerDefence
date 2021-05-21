#include "DamageBuff.h"

#include "TowerDefence/Actors/Visual/NumerialWidget.h"
#include "TowerDefence/Entities/Base/Entity.h"

UDamageBuff::UDamageBuff(const FObjectInitializer& ObjectInitializer)
	:UBuffEntity(ObjectInitializer)
{
}

void UDamageBuff::OnChangeValue()
{
	auto TargetValue = ChangedValueOffest.Find(GET_MEMBER_NAME_STRING_CHECKED(FEntityParams, CurrentHP));
	if(TargetValue== nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("can not found changed Params '%s' in buffID %d!"),
			*GET_MEMBER_NAME_STRING_CHECKED(FEntityParams, CurrentHP), MyBuff->BuffID);
		return;
	}
	TSoftClassPtr<ANumerialWidget> NumerialWidgetActorClass(FSoftObjectPath(
TEXT("BlueprintGeneratedClass'/Game'")));
	if(!NumerialWidgetActorClass.LoadSynchronous())
	{
		UE_LOG(LogTemp, Error, TEXT("Can not found Target NumerialWidget In DamageBuff"));
		return;
	}
	ANumerialWidget* TargetWidget = ParentEntity->GetWorld()->SpawnActor<ANumerialWidget>(NumerialWidgetActorClass.LoadSynchronous());
	
}

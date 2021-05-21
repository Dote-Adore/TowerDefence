#include "DamageBuff.h"

#include "TowerDefence/Entities/Base/Entity.h"

void UDamageBuff::OnChangeValue()
{
	auto TargetValue = ChangedValueOffest.Find(GET_MEMBER_NAME_STRING_CHECKED(FEntityParams, CurrentHP));
	if(TargetValue== nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("can not found changed Params '%s' in buffID %d!"),
			*GET_MEMBER_NAME_STRING_CHECKED(FEntityParams, CurrentHP), MyBuff->BuffID);
		return;
	}
}

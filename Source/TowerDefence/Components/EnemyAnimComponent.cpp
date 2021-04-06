﻿#include "EnemyAnimComponent.h"

#include "TowerDefence/Entities/Base/Entity.h"

void UEnemyAnimComponent::OnInit()
{
	Super::OnInit();
	const FEntityAnimation& Anims = ParentEntity->GetAnimations();
	FProperty* WalkProperty = AnimInst->GetClass()->FindPropertyByName(WalkAnimParamName);
	if(WalkProperty)
	{
		FObjectProperty* WalkObjectProperty = CastField<FObjectProperty>(WalkProperty);
		WalkObjectProperty->SetPropertyValue_InContainer(AnimInst, Anims.IdleAnim.LoadSynchronous());
	}
}
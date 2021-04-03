#include "AnimComponent.h"

#include "AITypes.h"
#include "TowerDefence/Entities/Base/Entity.h"
void UAnimComponent::BeginPlay()
{
    Super::BeginPlay();
    ParentEntity = Cast<AEntity>(GetOwner());
    check(ParentEntity);
    ParentEntity->OnEntityInitialized.BindUObject(this, &UAnimComponent::OnInit);
}

void UAnimComponent::OnInit()
{
    const FEntityAnimation& Anims = ParentEntity->GetAnimations();
    UAnimInstance* AnimInstace = ParentEntity->GetMesh()->GetAnimInstance();
    FProperty* IdleProperty = AnimInstace->GetClass()->FindPropertyByName(IdleAnimParamName);
    FObjectProperty* ObejctProperty = CastField<FObjectProperty>(IdleProperty);
    ObejctProperty->SetPropertyValue_InContainer(AnimInstace,  Anims.IdleAnim.Get());
}

#include "AnimComponent.h"

#include "AITypes.h"
#include "TowerDefence/Entities/Base/Entity.h"
void UAnimComponent::BeginPlay()
{
    Super::BeginPlay();
    ParentEntity = Cast<AEntity>(GetOwner());
    check(ParentEntity);
    ParentEntity->OnEntityInitialized.BindUObject(this, &UAnimComponent::OnInit);
    ParentEntity->OnAttackDelegate.BindUObject(this, &UAnimComponent::OnAttack);
    ParentEntity->OnDeathDelegate.BindUObject(this, &UAnimComponent::OnDeath);
}

void UAnimComponent::OnInit()
{
    const FEntityAnimation& Anims = ParentEntity->GetAnimations();
    AnimInst = ParentEntity->GetMesh()->GetAnimInstance();
    FProperty* IdleProperty = AnimInst->GetClass()->FindPropertyByName(IdleAnimParamName);
    if(IdleProperty)
    {
        FObjectProperty* ObejctProperty = CastField<FObjectProperty>(IdleProperty);
        ObejctProperty->SetPropertyValue_InContainer(AnimInst,  Anims.IdleAnim.LoadSynchronous());
    }
    FProperty* DeathProperty = AnimInst->GetClass()->FindPropertyByName(DeathAnimParamName);
    if(DeathProperty)
    {
        FObjectProperty* DetahObejctProperty = CastField<FObjectProperty>(DeathProperty);
        DetahObejctProperty->SetPropertyValue_InContainer(AnimInst, Anims.DeathAnim.LoadSynchronous());
    }

}

void UAnimComponent::OnAttack(int32 AttackIdx)
{
    UAnimSequenceBase* TargetAttackAnims =  ParentEntity->GetAnimations().AttackAnims[AttackIdx].LoadSynchronous();
    AnimInst->PlaySlotAnimationAsDynamicMontage(TargetAttackAnims, AttackAnimSlotName);
}

void UAnimComponent::OnDeath()
{
    FProperty* DeathProperty =  AnimInst->GetClass()->FindPropertyByName("IsDeath");
    FBoolProperty* BoolProperty = CastField<FBoolProperty>(DeathProperty);
    BoolProperty->SetPropertyValue_InContainer(AnimInst, true);
    
}

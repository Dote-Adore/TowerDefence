#include "AnimComponent.h"
#include "Animation/BlendSpace1D.h"
#include "AITypes.h"
#include "TowerDefence/Entities/Base/Entity.h"

UAnimComponent::UAnimComponent(const FObjectInitializer& ObjectInitializer)
    :UActorComponent(ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UAnimComponent::BeginPlay()
{
    Super::BeginPlay();
    ParentEntity = Cast<AEntity>(GetOwner());
    check(ParentEntity);
    ParentEntity->OnEntityInitialized.BindUObject(this, &UAnimComponent::OnInit);
    ParentEntity->OnAttackDelegate.BindUObject(this, &UAnimComponent::OnAttack);
    ParentEntity->OnDeathDelegate.BindUObject(this, &UAnimComponent::OnDeath);
}

void UAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(!AnimInst)
    {
        return;
    }
    float PlayRate = ParentEntity->GetCurrentEntityParams().AttackSpeedRate;
    if(AttackAnimMontage)
    {
        AnimInst->Montage_SetPlayRate(AttackAnimMontage, PlayRate);
    }
    FProperty* PlayRateProperty = AnimInst->GetClass()->FindPropertyByName(PlayRateName);
    if(PlayRateProperty)
    {
        FFloatProperty* PlayRateFloatProperty = CastField<FFloatProperty>(PlayRateProperty);
        if(PlayRateFloatProperty)
        {
            PlayRateFloatProperty->SetPropertyValue_InContainer(AnimInst, PlayRate);
        }
    }

}

void UAnimComponent::OnInit()
{
    const FEntityAnimation& Anims = ParentEntity->GetAnimations();
    AnimInst = ParentEntity->GetMesh()->GetAnimInstance();
    FProperty* IdleProperty = AnimInst->GetClass()->FindPropertyByName(IdleAnimParamName);
    if(IdleProperty)
    {
        FObjectProperty* ObejctProperty = CastField<FObjectProperty>(IdleProperty);
        ObejctProperty->SetPropertyValue_InContainer(AnimInst,  Anims.Idle_WalkAnim.LoadSynchronous());
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
    AttackAnimMontage = AnimInst->PlaySlotAnimationAsDynamicMontage(TargetAttackAnims, AttackAnimSlotName);
}

void UAnimComponent::OnDeath()
{
    FProperty* DeathProperty =  AnimInst->GetClass()->FindPropertyByName("IsDeath");
    FBoolProperty* BoolProperty = CastField<FBoolProperty>(DeathProperty);
    BoolProperty->SetPropertyValue_InContainer(AnimInst, true);
    
}

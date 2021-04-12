#include "Entity.h"
#include "Bullet.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/GlobalConfig.h"
#include "TowerDefence/Components/BuffComponent.h"
#include "TowerDefence/Components/AnimComponent.h"

AEntity::AEntity(const FObjectInitializer& ObjectInitializer)
    :ACharacter(ObjectInitializer)
{
    AnimInstanceClass = UAnimInstance::StaticClass();
    BuffComponent = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComp"));
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void AEntity::InitEntity(const FEntityParams& Params, const FEntityAnimation& Anims, FTransform TargetTransform,
                         const TArray<FBuff*>& BasePermanentBuffs)
{
    
    AnimComponent = NewObject<UAnimComponent>(this, GetAnimCompClass(), TEXT("AnimComp"));
    AnimComponent->RegisterComponent();
    // 初始化Mesh
    USkeletalMeshComponent* MeshComp = GetMesh();
    check(MeshComp);
    MeshComp->SetRelativeRotation(FRotator(0,0,-90));
    MeshComp->SetSkeletalMesh(Params.SkeletalMesh.LoadSynchronous());
    MeshComp->SetRenderCustomDepth(true);
    // 对初始值进行初始化
    const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
    MeshComp->SetAnimInstanceClass(AnimInstanceClass);
    SetActorTransform(TargetTransform);
    // 初始化攻击时视觉特效
    Animations = Anims;
    BaseEntityParams = Params;

    // 当前生命值点满
    BaseEntityParams.CurrentHP = BaseEntityParams.MaxHP;
    
    CurrentEntityParams = BaseEntityParams;
    for(auto Buff:BasePermanentBuffs)
    {
        BuffComponent->AddBuff(Buff);
    }
    // OnDeploy();
    if(BaseEntityParams.Attacks.Num()>=0)
    {
        CurrentHitIdx = 0;
        LeftHitTime = 0;
    }
    OnEntityInitialized.ExecuteIfBound();
}

void AEntity::Tick(float DeltaSeconds)
{
    // 首先看是否是在部署
    if(BaseEntityParams.DeployTime>=0)
    {
        BaseEntityParams.DeployTime -=DeltaSeconds;
        return;
    }
    // 这里将玩家当前的Hp限定为0~最大值
    CurrentEntityParams.CurrentHP = FMath::Clamp(CurrentEntityParams.CurrentHP, 0, CurrentEntityParams.MaxHP);
    // 攻击
    if(CurrentEntityParams.CurrentHP>0)
    {
        CalculateAttack(DeltaSeconds);
    }
    else
    {
        if(LeftDeathTime>=0)
        {
            LeftDeathTime -= DeltaSeconds;
        }
        else
        {
            Destroy();
        }
    }
}

void AEntity::BeginPlay()
{
    Super::BeginPlay();
}

void AEntity::BeginDestroy()
{
    Super::BeginDestroy();
    UE_LOG(LogTemp, Display ,TEXT("A Entity Destoryed"));
}

TSubclassOf<UAnimComponent> AEntity::GetAnimCompClass() const
{
    return UAnimComponent::StaticClass();
}


TSubclassOf<AEntity> AEntity::GetAttackTargetClass()
{
    return StaticClass();
}

void AEntity::CalculateAttackEntities()
{
    TArray<AActor*> FoundActors;

    switch (BaseEntityParams.EntityType)
    {
        // 范围攻击，查找攻击范围所有的目标
        case EEntityType::RangeAttack:
            CurrentAttackedEntities.Empty();
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetAttackEntityClass, FoundActors);
            for(AActor* Actor: FoundActors)
            {
                float Dist = FMath::Sqrt(FVector::DistSquaredXY(Actor->GetActorLocation(), GetActorLocation()));
                if(Dist <= BaseEntityParams.AttackRadius)
                {
                    CurrentAttackedEntities.Add(Cast<AEntity>(Actor));
                }
            }
            break;
        // 辅助，对己方发动
        case EEntityType::Assist:
            CurrentAttackedEntities.Empty();
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), GetClass(), FoundActors);
        for(AActor* Actor: FoundActors)
        {
            if(Actor == this)
                continue;
            float Dist = FMath::Sqrt(FVector::DistSquaredXY(Actor->GetActorLocation(), GetActorLocation()));
            if(Dist <= BaseEntityParams.AttackRadius)
            {
                CurrentAttackedEntities.Add(Cast<AEntity>(Actor));
            }
        }
            break;
        // 单个攻击，对敌方有效
        case EEntityType::SingleAttack:
            if(CurrentAttackedEntities.Num() > 0 && !CurrentAttackedEntities[0]->IsPendingKill())
            {
                float Distance = FMath::Sqrt(FVector::DistSquaredXY(CurrentAttackedEntities[0]->GetActorLocation(), GetActorLocation()));
                // 如果距离小于可攻击范围的的话就可以不用管
                if(Distance<= BaseEntityParams.AttackRadius)
                {
                    break;
                }
            }
            CurrentAttackedEntities.Empty();           
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetAttackEntityClass, FoundActors);
            // get the nearest actor
            AActor* NearestActor = nullptr;
            for(AActor* Actor: FoundActors)
            {
                if(!NearestActor)
                {
                    NearestActor = Actor;
                    continue;
                }
                float Dist =FMath::Sqrt( FVector::DistSquaredXY(Actor->GetActorLocation(), GetActorLocation()));
                float NearestDist = FMath::Sqrt(FVector::DistSquaredXY(NearestActor->GetActorLocation(), GetActorLocation()));
                if(Dist<NearestDist)
                {
                    NearestActor = Actor;
                }
            }
            if(NearestActor == nullptr)
                return;
            // 如果找到最接近的actor并且在攻击范围内，则将这个actor放到受攻击列表中
            float Dist = FVector::DistSquaredXY(NearestActor->GetActorLocation(), GetActorLocation());
            if(NearestActor&&
                FMath::Sqrt(Dist)<=CurrentEntityParams.AttackRadius)
            {
                CurrentAttackedEntities.Add(Cast<AEntity>(NearestActor));
            }
            break;
    }
}
void AEntity::CalculateAttack(float DeltaSeconds)
{
    const FEntityHitAttack* CurrentAttack;
    if(BaseEntityParams.Attacks.Num()<=0)
    {
        CurrentHitIdx = BaseEntityParams.Attacks.Num() - 1;
        LeftHitTime = 0;
        return;
    }
    // 切换招式
    if(LeftHitTime <= 0)
    {
        CurrentHitIdx = (CurrentHitIdx+1)%BaseEntityParams.Attacks.Num();
        CurrentAttack = &BaseEntityParams.Attacks[CurrentHitIdx];
        LeftHitTime = CurrentAttack->Stiff;
        OnAttack();
    }
    LeftHitTime -= DeltaSeconds;
}

void AEntity::OnAttack()
{
    // 计算可以攻击的目标对象组
    CalculateAttackEntities();
    UE_LOG(LogTemp, Display, TEXT("%s: Find Attack Nums: %d"), *GetName(), CurrentAttackedEntities.Num());
    const FEntityHitAttack& CurrentAttack = BaseEntityParams.Attacks[CurrentHitIdx];
    const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
    // 当前没有可攻击对象，则idle
    if(CurrentAttackedEntities.Num() == 0)
    {
        CurrentHitIdx = 0;
        OnIdleDelegate.ExecuteIfBound();
    }
    else
    {
        OnAttackDelegate.ExecuteIfBound(CurrentHitIdx);
    }
    for(AEntity* AttackEntity: CurrentAttackedEntities)
    {
        ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(CurrentAttack.BulletClass);
        UE_LOG(LogTemp, Display, TEXT("Spawn Bullet"));
        Bullet->File(this, AttackEntity, CurrentAttack);
    }
}

void AEntity::OnDamage(int32 DamageValue,  const FBuff* Buff)
{
    // 如果已经没血了，就不用再收到伤害了
    if(CurrentEntityParams.CurrentHP <= 0)
        return;
    // 减掉防御力
    int32 FinalAttack = FMath::Max(0, DamageValue - CurrentEntityParams.Defence);
    CurrentEntityParams.CurrentHP -= FinalAttack;
    if(Buff)
        BuffComponent->AddBuff(Buff);
    // 如果伤害为0，表示没有收到伤害，则不需要发送Damage事件
    if(DamageValue > 0)
    {
        OnDamageDelegate.ExecuteIfBound();
    }
    if(CurrentEntityParams.CurrentHP <=0)
    {
        OnDeath();
    }
}
void AEntity::OnDeath()
{
    const UGlobalConfig* Config = GetDefault<UGlobalConfig>();
    OnDeathDelegate.ExecuteIfBound();
}
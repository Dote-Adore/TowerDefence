#include "Entity.h"

#include "Kismet/GameplayStatics.h"

void AEntity::InitEntity(const FEntityParams& Params, const FEntityAnimation& Anims, FTransform TargetTransform,
        const TArray<FBuff>& BasePermanentBuffs)
{

     // 初始化Mesh
     USkeletalMeshComponent* MeshComp = GetMesh();
     check(MeshComp);
     MeshComp->SetSkeletalMesh(Params.SkeletalMesh.LoadSynchronous());
     // 对初始值进行初始化
     SetActorTransform(TargetTransform);
    // 初始化攻击时视觉特效
    Animations = Anims;
    
     BaseEntityParams = Params;
  for(const FBuff& Item: BasePermanentBuffs)
  {
      for(const FBuffAtom& BuffParam:Item.EffectParams)
      {
         FProperty* TargetProerty = BaseEntityParams.StaticStruct()->FindPropertyByName(BuffParam.TargetPropertyName);
          if(TargetProerty)
          {
             float* TargetValue = TargetProerty->ContainerPtrToValuePtr<float>(BaseEntityParams.StaticStruct());
              if(TargetValue)
              {
                  switch(Item.BuffType)
                  {
                      case EBuffType::Addition:
                          *TargetValue += BuffParam.TargetValue;
                      break;
                      case EBuffType::Magnification:
                          *TargetValue*=BuffParam.TargetValue;
                      break;
                  }
                  continue;
              }
              int32* TargetIntValue = TargetProerty->ContainerPtrToValuePtr<int32>(BaseEntityParams.StaticStruct());
              if(TargetIntValue)
              {
                  switch(Item.BuffType)
                  {
                  case EBuffType::Addition:
                      *TargetValue += BuffParam.TargetValue;
                      break;
                  case EBuffType::Magnification:
                      *TargetValue *= BuffParam.TargetValue;
                      break;
                  }
              }
          }
      }
  }
    
    CurrentEntityParams = BaseEntityParams;
}

void AEntity::Tick(float DeltaSeconds)
{
     // 首先看是否是在部署
     if(BaseEntityParams.DeployTime>=0)
     {
         BaseEntityParams.DeployTime -=DeltaSeconds;
         return;
     }
     // 攻击
    //if(!CurrentHP)
    {
        Attack(DeltaSeconds);
    }
    //else
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
    // OnDeploy();
    if(BaseEntityParams.Attacks.Num()>=0)
    {
        CurrentHitIdx = 0;
        LeftHitTime = BaseEntityParams.Attacks[CurrentHitIdx].Stiff;
    }
    
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
                float Dist = FVector::DistSquaredXY(Actor->GetActorLocation(), GetActorLocation());
                if(Dist <= BaseEntityParams.AttackRadius)
                {
                    CurrentAttackedEntities.Add(Cast<AEntity>(Actor));
                }
            }
            break;
        // 辅助，对己方发动
        case EEntityType::Assist:
            CurrentAttackedEntities.Empty();
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), FoundActors);
        for(AActor* Actor: FoundActors)
        {
            float Dist = FVector::DistSquaredXY(Actor->GetActorLocation(), GetActorLocation());
            if(Dist <= BaseEntityParams.AttackRadius)
            {
                CurrentAttackedEntities.Add(Cast<AEntity>(Actor));
            }
        }
            break;
        // 单个攻击，对敌方有效
        case EEntityType::SingleAttack:
            if(CurrentAttackedEntities.Num() > 0)
            {
                float Distance = FVector::DistSquaredXY(CurrentAttackedEntities[0]->GetActorLocation(), GetActorLocation());
                // 如果距离小于可攻击范围的的话就可以不用管
                if(Distance> BaseEntityParams.AttackRadius)
                {
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
                        float Dist = FVector::DistSquaredXY(Actor->GetActorLocation(), GetActorLocation());
                        float NearestDist = FVector::DistSquaredXY(NearestActor->GetActorLocation(), GetActorLocation());
                        if(Dist<NearestDist)
                        {
                           NearestActor = Actor;
                        }
                    }
                    // 如果找到最接近的actor并且在攻击范围内，则将这个actor放到受攻击列表中
                    if(NearestActor&&
                        FVector::DistSquaredXY(NearestActor->GetActorLocation(), GetActorLocation())<=BaseEntityParams.AttackRadius)
                    {
                        CurrentAttackedEntities.Add(Cast<AEntity>(NearestActor));
                    }
                    
                }
            }
            break;
    }
}

void AEntity::Attack(float DeltaSeconds)
{
    const FEntityHitAttack* CurrentAttack;
    if(BaseEntityParams.Attacks.Num()<=0)
        return;
    // 切换招式
    if(LeftHitTime <= 0)
    {
        CurrentHitIdx = (CurrentHitIdx+1)%BaseEntityParams.Attacks.Num();
        CurrentAttack = &BaseEntityParams.Attacks[CurrentHitIdx];
        LeftHitTime = CurrentAttack->Stiff;
        OnAttack();
    }
    LeftHitTime -= LeftHitTime-DeltaSeconds;
    
    
}

void AEntity::OnAttack()
{
    // 计算可以攻击的目标对象组
    CalculateAttackEntities();
    const FEntityHitAttack& CurrentAttack = BaseEntityParams.Attacks[CurrentHitIdx];
    for(AEntity* AttackEntity: CurrentAttackedEntities)
    {
        
    }
    // 播放动画
}

void AEntity::OnDamage(int32 DamageValue,  const FBuff& Buff)
{
}

void AEntity::AddBuff(const FBuff& Buff)
{
    
}

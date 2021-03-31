#include "Entity.h"

#include "Kismet/GameplayStatics.h"

void AEntity::InitEntity(const FEntity& Params, FTransform TargetTransform, const TArray<FBuff>& BasePermanentBuffs)
{

     // 初始化Mesh
     USkeletalMeshComponent* MeshComp = GetMesh();
     check(MeshComp);
     MeshComp->SetSkeletalMesh(Params.SkeletalMesh.LoadSynchronous());
     // 对初始值进行初始化

     
     SetActorTransform(TargetTransform);

     
     BaseEntityParams = Params;
  for(const FBuff& Item: BasePermanentBuffs)
  {
      for(const FBuffAtom& BuffParam:Item)
      {
         FProperty* TargetProerty = BaseEntityParams.StaticStruct()->FindPropertyByName(BuffParam.TargetPropertyName);
          if(TargetProerty)
          {
             float* TargetValue = TargetProerty->ContainerPtrToValuePtr<float>(BaseEntityParams.StaticStruct());
              if(TargetValue)
              {
                  switch(Item.BuffType)
                  {
                      case FBuff::Addition:
                          *TargetValue += BuffParam.TargetValue;
                      break;
                      case FBuff::Magnification:
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
                  case FBuff::Addition:
                      *TargetValue += BuffParam.TargetValue;
                      break;
                  case FBuff::Magnification:
                      *TargetValue *= BuffParam.TargetValue;
                      break;
                  }
              }
          }
      }
  }
}

void AEntity::Tick(float DeltaSeconds)
{
     // 首先看是否是在部署
     if(BaseEntityParams.DeployTime>0)
     {
         BaseEntityParams.DeployTime -=DeltaSeconds;
         OnDeploy();
         return;
     }
     // 攻击
     
}

void AEntity::CalculateAttackEntities()
{
    switch (BaseEntityParams.EntityType)
    {
        case EEntityType::RangeAttack:
            break; 
        case EEntityType::Assist:
            break;
        case EEntityType::SingleAttack:
            if(CurrentAttackedEntities.Num() > 0)
            {
                float Distance = FVector::DistSquaredXY(CurrentAttackedEntities[0]->GetActorLocation(), GetActorLocation());
                // 如果距离小于可攻击范围的的话就可以不用管
                if(Distance> BaseEntityParams.AttackRadius)
                {
                    CurrentAttackedEntities.Empty();
                    TArray<AActor*> FoundActors;
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

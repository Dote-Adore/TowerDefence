#pragma once
#include "Buff.h"
#include "GameFramework/Character.h"
#include "Interface/EntityAppearanceInterface.h"
#include "UObject/ObjectMacros.h"


#include "Entity.generated.h"


class ABullet;
class UAnimationAsset;
UENUM(BlueprintType)
enum class EEntityType
{
    // 一对一攻击
    SingleAttack,
    // 范围攻击
    RangeAttack,
    // 辅助，帮助队友的
    Assist
};

// 炮塔攻击的连招参数
USTRUCT(BlueprintType)
struct FEntityHitAttack
{
    GENERATED_BODY()
    float Radius;
    // 攻击率，每一段攻击对应总攻击的百分比
    float AttackRate;
    // 攻击时间
    float HitTime;
    // 硬直
    float Stiff;
    // 攻击的时候buff
    FBuff Buff;
    // 发出的子弹
    TSoftObjectPtr<ABullet> Bullet;
};

// 基础参数
USTRUCT(BlueprintType)
struct FEntity
{
    GENERATED_BODY()
    UPROPERTY()
    int32 ID;
    UPROPERTY()
    FName DisplayName;
    UPROPERTY()
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;
    UPROPERTY()
    EEntityType EntityType;
        
    // 部署时间
    UPROPERTY()
    int32 DeployTime = 1.f;
    
   
    // 基础攻击值
    UPROPERTY()
    int32 Attack;
    // 生命值
    UPROPERTY()
    int32 HP;
    // 防御值
    UPROPERTY()
    int32 Defence;
    // 攻击速率
    UPROPERTY()
    float AttackSpeedRate = 1.f;
    UPROPERTY()
    float AttackRadius;
    
    // 暴击率
    UPROPERTY()
    float CritRate;
    // 暴击伤害(百分比)
    UPROPERTY()
    float CritDamageRate;


    // 列表的作用是用来连招用
    UPROPERTY()
    TArray<FEntityHitAttack> Attacks;
};


UCLASS(Blueprintable)
class AEntity: public ACharacter,public
IEntityAppearanceInterface
{
    GENERATED_BODY()
public:
    // 目标攻击的Entity类型;
    UPROPERTY()
    TSubclassOf<AEntity> TargetAttackEntityClass;
    
    
    // 初始化实体
    void InitEntity(const FEntity& Params, FTransform TargetTransform,
        const TArray<FBuff>& BasePermanentBuffs);
    void Tick(float DeltaSeconds) override;
private:
    FEntity BaseEntityParams;
    TArray<AEntity*> CurrentAttackedEntities;
    
     void CalculateAttackEntities();

    
    // 连招相关
    
};

#pragma once
#include "Buff.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"


#include "Entity.generated.h"


class ABullet;
class UAnimationAsset;
UENUM()
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
    UPROPERTY(EditAnywhere)
    float Radius;
    // 攻击率，每一段攻击对应总攻击的百分比
    UPROPERTY(EditAnywhere)
    float AttackRate;
    // 攻击时间
    UPROPERTY(EditAnywhere)
    float AttackTime;
    // 硬直
    UPROPERTY(EditAnywhere)
    float Stiff;
    // 攻击的时候buff
    UPROPERTY(EditAnywhere)
    FBuff Buff;
    // 发出的子弹
    UPROPERTY(EditAnywhere)
    TSubclassOf<ABullet> BulletClass;
};

// 基础参数
USTRUCT(BlueprintType)
struct FEntityParams:public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere)
    FName DisplayName;
    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;
    UPROPERTY(EditAnywhere)
    EEntityType EntityType;
        
    // 部署时间
    UPROPERTY(EditAnywhere)
    float DeployTime = 1.f;
   
    // 基础攻击值
    UPROPERTY(EditAnywhere)
    int32 Attack;
    // 生命值
    UPROPERTY(EditAnywhere)
    int32 HP;
    // 防御值
    UPROPERTY(EditAnywhere)
    int32 Defence;
    // 攻击速率
    UPROPERTY(EditAnywhere)
    float AttackSpeedRate = 1.f;
    UPROPERTY(EditAnywhere)
    float AttackRadius;
    
    // 暴击率
    UPROPERTY(EditAnywhere)
    float CritRate;
    // 暴击伤害(百分比)
    UPROPERTY(EditAnywhere)
    float CritDamageRate;


    // 列表的作用是用来连招用
    UPROPERTY(EditAnywhere)
    TArray<FEntityHitAttack> Attacks;
};

// 攻击的时候的视觉特效
USTRUCT(BlueprintType)
struct FHitAttackVisualEffect
{
    GENERATED_BODY()
    int32 EntityID;
    TArray<TSoftObjectPtr<UAnimationAsset>> Anims;
};


UCLASS(Blueprintable)
class AEntity: public ACharacter
// ,public IEntityAppearanceInterface
{
    GENERATED_BODY()
public:
    // 目标攻击的Entity类型;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AEntity> TargetAttackEntityClass;
    
    
    // 初始化实体
    void InitEntity(const FEntityParams& Params, const FHitAttackVisualEffect& InAttackVisualEffect, FTransform TargetTransform,
        const TArray<FBuff>& BasePermanentBuffs);
    void Tick(float DeltaSeconds) override;
    const FEntityParams& GetBaseEntityParams() const { return BaseEntityParams;};
    virtual void BeginPlay() override;
    virtual void OnAttack();
private:
    FEntityParams BaseEntityParams;
    TArray<AEntity*> CurrentAttackedEntities;
    FHitAttackVisualEffect HitAttackVisualEffect;

    
    void CalculateAttackEntities();
    void Attack(float DeltaSeconds);    
    // 连招
    // 当前连招使用哪个id
    int32 CurrentHitIdx = 0;
    // 该招式的剩余时间
    float LeftHitTime;

    
    bool IsDeath = false;
    float LeftDeathTime = 1.f;
};

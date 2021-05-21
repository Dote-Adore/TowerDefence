#pragma once
#include "Buff.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"


#include "Entity.generated.h"

class UBlendSpace1D;
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
    int32 BuffID;
    // 发出的子弹
    UPROPERTY(EditAnywhere)
    TSubclassOf<ABullet> BulletClass;
};

// 基础参数
USTRUCT(BlueprintType)
struct FEntityParams:public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 EntityID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EEntityType EntityType;
    // 分类
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    FName Category;
    // 部署点数（如果时敌人，配置的是被击败时获得的部署点数）
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 DeployPoints;
    // 部署时间
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DeployTime = 1.f;
   
    // 基础攻击值
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Attack;
    // 当前生命值
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentHP;
    // 策划配表的最大生命值
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxHP;
    // 防御值
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Defence;
    // 攻击速率
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AttackSpeedRate = 1.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
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
struct FEntityAnimation:public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere)
    TSubclassOf<UAnimInstance> AnimInstance;
    UPROPERTY(EditAnywhere)
    int32 EntityID;
    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UBlendSpace1D> Idle_WalkAnim;
    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UAnimSequenceBase> DeathAnim;
    UPROPERTY(EditAnywhere)
    TArray<TSoftObjectPtr<UAnimSequenceBase>> AttackAnims;
};


// 攻击消息，参数是攻击下标
DECLARE_DELEGATE_OneParam(FOnAttackDelegate, int32);
DECLARE_DELEGATE(FOnEntityInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageDelegate, int32, DamageValue, FTransform, DamageTransform);
DECLARE_DELEGATE(FOnIdleDelegate);
DECLARE_DELEGATE(FOnDeathDelegate);
UCLASS(Blueprintable)
class AEntity: public ACharacter
// ,public IEntityAppearanceInterface
{
    GENERATED_BODY()
public:
    AEntity(const FObjectInitializer& ObjectInitializer);
    // 目标攻击的Entity类型;
    
    FOnAttackDelegate OnAttackDelegate;
    FOnEntityInitialized OnEntityInitialized;
    FOnIdleDelegate OnIdleDelegate;
    FOnDeathDelegate OnDeathDelegate;
    UPROPERTY(BlueprintAssignable)
    FOnDamageDelegate OnDamageDelegate;


    UPROPERTY(Transient)
    TArray<AEntity*> CurrentAttackedEntities;


    
    // 初始化实体
    void InitEntity(const FEntityParams& Params, const FEntityAnimation& Anims, FTransform TargetTransform,
        const TArray<FBuff*>& BasePermanentBuffs);
    void Tick(float DeltaSeconds) override;
    UFUNCTION(BlueprintCallable)
    FEntityParams& GetCurrentEntityParams() { return CurrentEntityParams;};
    const FEntityAnimation& GetAnimations() const {return Animations; };
    virtual void BeginPlay() override;
    virtual void BeginDestroy() override;
    
    virtual void OnAttack();
    virtual void OnDamage(int32 DamageValue, const FBuff* Buff, FTransform DamageFransform );
    virtual void OnDeath();
    bool IsDeath();
protected:
    class UBuffComponent* BuffComponent;
    class UAnimComponent* AnimComponent;
    class UDamageComponent* DamageComponent;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AEntity> TargetAttackEntityClass;
    UPROPERTY(EditDefaultsOnly)
    float LeftDeathTime = 2.f;
    virtual TSubclassOf<UAnimComponent> GetAnimCompClass() const;
    virtual TSubclassOf<AEntity> GetAttackTargetClass();
    
private:
    UPROPERTY(Transient)
    FEntityParams CurrentEntityParams;
    
    FEntityAnimation Animations;    
    void CalculateAttackEntities();
    // 计算buff到主数值中
    void CalculateAttack(float DeltaSeconds);
    void SetMeshMaterialsColorParams(FName ParamName, FLinearColor Color);
    void OnSpawnAttackBullet(int32 AttackIdx, TArray<AEntity*> TargetEntities);
    // 连招
    // 当前连招使用哪个id
    int32 CurrentHitIdx = 0;
    // 该招式的剩余时间
    float LeftHitTime;
    FTimerHandle DamageEffectTimerHandle;
    //攻击有一个前摇时间，这里进行延时
    FTimerHandle AttackTimerHandle;
};



#include "AnimComponent.h"

#include "EnemyAnimComponent.generated.h"



UCLASS(Blueprintable)
class UEnemyAnimComponent:public UAnimComponent
{
	GENERATED_BODY()
public:
	FName WalkAnimParamName = "Idle_WalkAnim";
protected:
	virtual void OnInit() override;
};
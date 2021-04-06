#include "AnimComponent.h"

#include "EnemyAnimComponent.generated.h"



UCLASS(Blueprintable)
class UEnemyAnimComponent:public UAnimComponent
{
	GENERATED_BODY()
public:
	FName WalkAnimParamName = "WalkAnim";
	virtual void OnInit() override;
};
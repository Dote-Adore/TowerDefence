#pragma once
#include "StateMachineComponent.generated.h"

class UState
{
public:
	virtual ~UState() = default;
	UState();
	UState(class UStateMachineComponent* InStateMachine);
	virtual void Tick(float DeltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual FName GetStateName() = 0;
protected:
	class UStateMachineComponent* OwnerStateMachine;
};

UCLASS()
class UStateMachineComponent:public UActorComponent
{
	GENERATED_BODY()
public:
	UStateMachineComponent(const FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeState(FName TargetName);
	void RegisterState(TSharedPtr<UState> State);
private:
	TSharedPtr<UState> CurrentState;
	TMap<FName, TSharedPtr<UState>> StateMap;
};

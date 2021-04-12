#include "StateMachineComponent.h"


// UState::UState()
// {
// }

UState::UState(UStateMachineComponent* InStateMachine)
	:OwnerStateMachine(InStateMachine)
{
}

UStateMachineComponent::UStateMachineComponent(const FObjectInitializer& ObjectInitializer)
	:UActorComponent(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(CurrentState.IsValid())
	{
		CurrentState->Tick(DeltaTime);
	}
	
}

void UStateMachineComponent::ChangeState(FName TargetName)
{
	if(CurrentState.IsValid())
	{
		CurrentState->OnExit();
	}
	auto TargetFoundState= StateMap.Find(TargetName);
	if(TargetFoundState)
	{
		CurrentState = *TargetFoundState;
		CurrentState->OnEnter();
	}
	else
	{
		CurrentState = nullptr;
	}
}

void UStateMachineComponent::RegisterState(TSharedPtr<UState> State)
{
	StateMap.Add(State->GetStateName(), State);
}

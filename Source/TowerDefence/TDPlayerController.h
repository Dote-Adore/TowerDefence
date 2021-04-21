#pragma once
#include "GameFramework/PlayerController.h"
#include "TDPlayerController.generated.h"


class ABaseTile;
class ATDPlayerController;
DECLARE_EVENT_OneParam(ATDPlayerController, FOnTileSelectedEvent, ABaseTile*)
UCLASS()
class ATDPlayerController:public APlayerController
{
	GENERATED_BODY()
public:
	ATDPlayerController(const FObjectInitializer& ObjectInitializer);
	virtual void SetupInputComponent() override;
	static FOnTileSelectedEvent OnTileSelectedEvent;

private:
	UFUNCTION()
	void OnMouseLeftBtnClicked();
};

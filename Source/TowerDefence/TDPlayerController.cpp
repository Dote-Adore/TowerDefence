#include "TDPlayerController.h"

#include "Entities/Base/Tile.h"

FOnTileSelectedEvent ATDPlayerController::OnTileSelectedEvent = FOnTileSelectedEvent();
ATDPlayerController::ATDPlayerController(const FObjectInitializer& ObjectInitializer)
	:APlayerController(ObjectInitializer)
{
	bEnableClickEvents = true;
	bShowMouseCursor = true;
}

void ATDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("MouseLeftClicked", IE_Pressed, this, &ATDPlayerController::OnMouseLeftBtnClicked);
}

void ATDPlayerController::OnMouseLeftBtnClicked()
{
	UE_LOG(LogTemp, Display, TEXT("PlayerController: MouseLeftBtnClicked"));
	FHitResult Result;
	GetHitResultUnderCursorForObjects({EObjectTypeQuery::ObjectTypeQuery2}, false, Result);
	if(Result.GetActor())
	{
		AActor* Target = Result.GetActor();
		UE_LOG(LogTemp, Display,TEXT("PlayerController: Succes Hit Res! '%s'"), *Target->GetName());
		if(Target->GetClass()->IsChildOf(ABaseTile::StaticClass()))
		{
			ABaseTile* TileActor = Cast<ABaseTile>(Target);
			OnTileSelectedEvent.Broadcast(TileActor);
		}
	}
}

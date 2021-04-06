#include "Tile.h"
#include "Components/BoxComponent.h"

ABaseTile::ABaseTile(const FObjectInitializer& ObjectInitializer)
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(BoxSize,BoxSize,BoxSize));
}

#include "Tile.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"

ABaseTile::ABaseTile(const FObjectInitializer& ObjectInitializer)
{

	UBillboardComponent* SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> DecalTexture;
		FConstructorStatics() : DecalTexture(TEXT("/Engine/Editor/Slate/Icons/AssetIcons/Sphere_64x.Sphere_64x")) {}
	};
	static FConstructorStatics ConstructorStatics;
	if (SpriteComponent)
	{
		SpriteComponent->Sprite = ConstructorStatics.DecalTexture.Get();
		SpriteComponent->SetupAttachment(RootComponent);
		SpriteComponent->SetUsingAbsoluteScale(true);
		SpriteComponent->bIsScreenSizeScaled = true;
		SpriteComponent->bReceivesDecals = false;
		SpriteComponent->bHiddenInGame = true;
		SpriteComponent->ScreenSize = 2.f;
	}
	SetRootComponent(SpriteComponent);
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeLocation(FVector(0,0,-BoxSize+HeightOffest));
	BoxComponent->SetBoxExtent(FVector(BoxSize,BoxSize,BoxSize));
}

void ABaseTile::Deploy(AEntity* Entity)
{
	// if(TileType == Path || TileType == Deployable)
	
}

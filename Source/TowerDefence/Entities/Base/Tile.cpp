#include "Tile.h"

#include "Entity.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

const float ABaseTile::BoxSize = 50.f;
ABaseTile::ABaseTile(const FObjectInitializer& ObjectInitializer)
	:AActor(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	UBillboardComponent* SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	SelectedPlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectedUIComp"));
	SelectedPlaneComponent->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")));
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
	BoxComponent->SetHiddenInGame(false);

	SelectedPlaneComponent->SetupAttachment(RootComponent);
	// BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);	
}
void ABaseTile::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->ShapeColor = DebugColor.ToFColor(true);
	SlectedShowMID = SelectedPlaneComponent->CreateDynamicMaterialInstance(0,
        LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInterface'/Game/Res/Materials/Tiles/M_BaseTileSelectedMat_Inst.M_BaseTileSelectedMat_Inst'")));
	SelectedPlaneComponent->SetRelativeLocation(FVector(0, 0, HeightOffest));
	SlectedShowMID->SetVectorParameterValue("Color", FLinearColor(0,0,0,0));

}

void ABaseTile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// SlectedShowMID->SetVectorParameterValue("Color", DebugColor);
}

void ABaseTile::OnConstruction(const FTransform& Transform)
{
	BoxComponent->SetRelativeLocation(FVector(0,0,-BoxSize+HeightOffest));
	BoxComponent->ShapeColor = DebugColor.ToFColor(true);	
}

void ABaseTile::SetDeployEntity(AEntity* TargetEntity)
{
	CurrentDeployEntity = TargetEntity;
}

bool ABaseTile::CanDeploy(FName TargetCategory)
{
	if(TileType == ETileType::Disable||
       TileType == ETileType::End||
       TileType == ETileType::Start||
       IsValid(CurrentDeployEntity)||
       DeployableEntityTypes.Find(TargetCategory) == INDEX_NONE)
	{
		return false;
	}
	return true;
}

void ABaseTile::Deploy(AEntity* Entity)
{
	// if(TileType == Path || TileType == Deployable)
	
}

void ABaseTile::ChangePlaneColor(FLinearColor InColor)
{
	SlectedShowMID->SetVectorParameterValue("Color", InColor);
}

FVector ABaseTile::GetSpawnEntityLocation() const
{
	return GetActorLocation()+FVector(0,0,HeightOffest);
}

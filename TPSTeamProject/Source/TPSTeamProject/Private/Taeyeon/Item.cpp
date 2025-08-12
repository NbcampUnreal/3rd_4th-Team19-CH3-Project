#include "Taeyeon/Item.h"

#include "Character/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Taeyeon/ItemComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	ItemStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ItemStaticMeshComp->SetupAttachment(RootComponent);
	
	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComponent"));
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}
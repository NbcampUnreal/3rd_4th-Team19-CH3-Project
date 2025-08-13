#include "Taeyeon/ItemComponent.h"

#include "Character/ShooterCharacter.h"
#include "Taeyeon/InventoryComponent.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Amount = 1;
	ItemName = NAME_None;
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemComponent::Interact(ACharacter* PlayerCharacter)
{
	check(!ItemName.IsNone());
	
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(PlayerCharacter);
	if (ShooterCharacter == nullptr) return;

	UInventoryComponent* InventorySystem = ShooterCharacter->GetComponentByClass<UInventoryComponent>();
	check(InventorySystem);
	
	InventorySystem->AddItem(ItemName, Amount);
	
	GetOwner()->Destroy();
}


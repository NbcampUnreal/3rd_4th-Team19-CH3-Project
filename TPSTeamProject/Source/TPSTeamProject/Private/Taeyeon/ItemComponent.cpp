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

void UItemComponent::Interact(AActor* Interactor)
{
	check(!ItemName.IsNone());
	
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(Interactor);
	if (ShooterCharacter == nullptr) return;

	UInventoryComponent* InventorySystem = ShooterCharacter->GetComponentByClass<UInventoryComponent>();
	if (InventorySystem == nullptr) return;

	const bool bIsAddItemSuccess = InventorySystem->AddItem(ItemName, Amount);
	if (bIsAddItemSuccess)
	{
		GetOwner()->Destroy();
	}
}


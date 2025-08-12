#include "Taeyeon/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItem(FName ItemID, int32 Quantity)
{
	
}

void UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
}


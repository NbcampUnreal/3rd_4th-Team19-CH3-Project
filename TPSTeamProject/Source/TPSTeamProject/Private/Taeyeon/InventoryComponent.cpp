#include "Taeyeon/InventoryComponent.h"
#include "GameData/ItemDataStruct.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Inventory.SetNum(InventorySize);
}

bool UInventoryComponent::AddItem(FName ItemName, int32 Amount)
{
	if (!ItemDataTable || ItemName.IsNone() || Amount <= 0)
	{
		return false;
	}

	FItemDataStruct* ItemData = ItemDataTable->FindRow<FItemDataStruct>(ItemName, TEXT("Find Item Data"));
	if (!ItemData)
	{
		return false;
	}

	int32 AddAmount = Amount;

	if (ItemData->StackSize > 1)
	{
		for (FInventorySlot& Slot : Inventory)
		{
			if (Slot.ItemName == ItemName && Slot.Amount < ItemData->StackSize)
			{
				const int32 AmountLeft = ItemData->StackSize - Slot.Amount;
				const int32 RealAddAmount = FMath::Min(AddAmount, AmountLeft);

				Slot.Amount += RealAddAmount;
				AddAmount -= RealAddAmount;

				if (AddAmount <= 0) break;
			}
		}
	}

	if (AddAmount > 0)
	{
		for (FInventorySlot& Slot : Inventory)
		{
			if (Slot.ItemName.IsNone())
			{
				const int32 RealAddAmount = FMath::Min(AddAmount, ItemData->StackSize);

				Slot.ItemName = ItemName;
				Slot.Amount = RealAddAmount;
				AddAmount -= RealAddAmount;

				if (AddAmount <= 0) break;
			}
		}
	}

	if (AddAmount < Amount)
	{
		OnInventoryUpdated.Broadcast();
	}

	return AddAmount == 0;
}

void UInventoryComponent::RemoveItem(FName ItemName, int32 Amount)
{
	if (ItemName.IsNone() || Amount <= 0)
	{
		return;
	}

	int32 RemoveAmount = Amount;

	for (int32 i = Inventory.Num() - 1; i >= 0; --i)
	{
		if (Inventory[i].ItemName == ItemName)
		{
			const int32 RealRemoveAmount = FMath::Min(Inventory[i].Amount, RemoveAmount);

			Inventory[i].Amount -= RealRemoveAmount;
			RemoveAmount -= RealRemoveAmount;

			if (Inventory[i].Amount <= 0)
			{
				Inventory[i].ItemName = NAME_None;
				Inventory[i].Amount = 0;
			}

			if (RemoveAmount <= 0) break;
		}
	}

	if (RemoveAmount < Amount)
	{
		OnInventoryUpdated.Broadcast();
	}
}

#include "Taeyeon/InventorySlotWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameData/ItemDataStruct.h"
#include "Taeyeon/InventoryComponent.h"

void UInventorySlotWidget::UpdateSlot(const FInventorySlot& SlotData, const UDataTable* ItemDataTable)
{
	if (!ItemDataTable) return;

	if (SlotData.ItemName.IsNone())
	{
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		ItemStackBox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		const FItemDataStruct* ItemData = ItemDataTable->FindRow<FItemDataStruct>(SlotData.ItemName, TEXT(""));
		if (ItemData)
		{
			ItemImage->SetBrushFromTexture(ItemData->ItemIconTexture.LoadSynchronous());
			ItemImage->SetVisibility(ESlateVisibility::Visible);

			if (SlotData.Amount > 1)
			{
				ItemStackText->SetText(FText::AsNumber(SlotData.Amount));
				ItemStackBox->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				ItemStackBox->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

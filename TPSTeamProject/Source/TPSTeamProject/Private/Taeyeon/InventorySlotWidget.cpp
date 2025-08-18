#include "Taeyeon/InventorySlotWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameData/ItemDataStruct.h"

void UInventorySlotWidget::UpdateSlot(const FInventorySlot& SlotData, const UDataTable* ItemDataTable)
{
	CurrentSlotData = SlotData;

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

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (!CurrentSlotData.ItemName.IsNone())
		{
			OnSlotRightClicked.Broadcast(CurrentSlotData);
			return FReply::Handled();
		}
	}
	return FReply::Unhandled();
}

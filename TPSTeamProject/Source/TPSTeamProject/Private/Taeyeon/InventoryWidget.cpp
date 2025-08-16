#include "Taeyeon/InventoryWidget.h"

#include "Components/WrapBox.h"
#include "Taeyeon/InventoryComponent.h"
#include "Taeyeon/InventorySlotWidget.h"

void UInventoryWidget::RefreshInventory(UInventoryComponent* InventoryComp)
{
    if (!InventoryComp || !SlotWidgetClass || !WrapBox) return;

    WrapBox->ClearChildren();

    for (const FInventorySlot& SlotData : InventoryComp->GetInventory())
    {
        UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
        if (SlotWidget)
        {
            SlotWidget->UpdateSlot(SlotData, InventoryComp->GetItemDataTable());
            WrapBox->AddChildToWrapBox(SlotWidget);
        }
    }
}

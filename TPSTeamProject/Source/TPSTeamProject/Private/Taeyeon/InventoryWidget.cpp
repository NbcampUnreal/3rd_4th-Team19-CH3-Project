#include "Taeyeon/InventoryWidget.h"

#include "Character/ShooterCharacter.h"
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

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::I)
    {
        AShooterCharacter* PlayerCharacter = Cast<AShooterCharacter>(GetOwningPlayerPawn());
        if (PlayerCharacter)
        {
            PlayerCharacter->ToggleInventory();
            return FReply::Handled();
        }
    }
    return FReply::Unhandled();
}

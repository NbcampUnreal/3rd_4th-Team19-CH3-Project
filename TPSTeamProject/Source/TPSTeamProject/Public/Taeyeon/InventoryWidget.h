#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventorySlotWidget;
class UWrapBox;
class UInventoryComponent;

UCLASS()
class TPSTEAMPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void RefreshInventory(UInventoryComponent* InventoryComp);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWrapBox> WrapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InventoryUI")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;
};

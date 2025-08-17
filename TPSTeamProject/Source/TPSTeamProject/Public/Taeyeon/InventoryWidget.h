#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventorySlotWidget;
class UWrapBox;
class UInventoryComponent;

UCLASS(BlueprintType, Blueprintable)
class TPSTEAMPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void RefreshInventory(UInventoryComponent* InventoryComp);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWrapBox> WrapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InventoryUI")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Taeyeon/InventoryComponent.h"
#include "InventorySlotWidget.generated.h"

class USizeBox;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotRightClicked, const FInventorySlot&, SlotData);

UCLASS()
class TPSTEAMPROJECT_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateSlot(const FInventorySlot& SlotData, const UDataTable* ItemDataTable);

	UPROPERTY(BlueprintAssignable)
	FOnSlotRightClicked OnSlotRightClicked;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	FInventorySlot CurrentSlotData;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<USizeBox> ItemStackBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemStackText;
};

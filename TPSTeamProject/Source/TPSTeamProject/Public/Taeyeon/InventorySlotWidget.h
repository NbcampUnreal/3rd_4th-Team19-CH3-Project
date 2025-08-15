#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class USizeBox;
class UTextBlock;
class UImage;
struct FInventorySlot;

UCLASS()
class TPSTEAMPROJECT_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateSlot(const FInventorySlot& SlotData, const UDataTable* ItemDataTable);

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<USizeBox> ItemStackBox;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemStackText;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

struct FInventorySlot;
class UInventorySlotWidget;
class UWrapBox;
class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUseRequested, const FInventorySlot&, SlotData);

UCLASS(BlueprintType, Blueprintable)
class TPSTEAMPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void RefreshInventory(UInventoryComponent* InventoryComp);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	UFUNCTION()
	void OnSlotRightClickHandler(const FInventorySlot& SlotData);
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnItemUseRequested OnItemUseRequested;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWrapBox> WrapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InventoryUI")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;
};

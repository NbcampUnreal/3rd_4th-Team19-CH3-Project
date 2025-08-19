#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CrosshairWidget.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> W_Dot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> LineTop;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> LineBottom;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> LineLeft;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> LineRight;

	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	void UpdateCrosshairSize(const float Speed);
	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	void VisibleCrosshair(bool bIsVisible);
	
};

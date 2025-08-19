#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.h"
#include "CrosshairComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSTEAMPROJECT_API UCrosshairComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCrosshairComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE float GetAimSize() const { return AimSize; }
	FORCEINLINE void SetAimSize(float Size) { AimSize = Size; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UCrosshairWidget> CrosshairWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;

	UFUNCTION()
	void VisibleCrosshair(bool bIsVisible);


private:
	void UpdateCrosshair(float DeltaTime);

	float AimSize;
	float MaxAimSize;
	float MinAimSize;
};

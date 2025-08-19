#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectTweenComponent.generated.h"

class UCurveFloat;
class UCurveVector;
class UCurveLinearColor;
class UTimelineComponent;

UENUM(BlueprintType)
enum class EObjectTweenValueType : uint8
{
	Float,
	Boolean,
	Vector,
	LinearColor
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSTEAMPROJECT_API UObjectTweenComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UObjectTweenComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tween")
	EObjectTweenValueType ValueType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tween")
	TObjectPtr<UCurveFloat> CurveFloat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tween")
	TObjectPtr<UCurveVector> CurveVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tween")
	TObjectPtr<UCurveLinearColor> CurveLinearColor;

	UFUNCTION(BlueprintCallable)
	void AddFloatEvent(UObject* InObject, const FName& UpdateFunctionName, const FName& FinishedFunctionName = NAME_None);
	UFUNCTION(BlueprintCallable)
	void AddVectorEvent(UObject* InObject, const FName& UpdateFunctionName, const FName& FinishedFunctionName = NAME_None);
	UFUNCTION(BlueprintCallable)
	void AddLinearColorEvent(UObject* InObject, const FName& UpdateFunctionName, const FName& FinishedFunctionName = NAME_None);
	UFUNCTION(BlueprintCallable)
	void AddBoolEvent(UObject* InObject, const FName& UpdateFunctionName, float InTime);

	void PlayFromStart();
	void Reverse();
	void EndPlay(EEndPlayReason::Type);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tween")
	TObjectPtr<UTimelineComponent> TweenTimelineComp;

		
};

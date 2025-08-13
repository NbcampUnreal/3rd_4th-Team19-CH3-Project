#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseZone.generated.h"

class UBoxComponent;

UCLASS()
class TPSTEAMPROJECT_API ABaseZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseZone();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger")
	UBoxComponent* TriggerBox;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Blueprintable, Category = "Trigger")
	virtual void ExecuteTriggerAction(AActor* OtherActor);
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseZone.generated.h"

class UBoxComponent;
class UZoneChecker;
class UObjectTweenComponent;

UCLASS()
class TPSTEAMPROJECT_API ABaseZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseZone();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	TObjectPtr<AStaticMeshActor> DoorMeshActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectTween")
	TObjectPtr<UObjectTweenComponent> OpenDoorTween;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectTween")
	TObjectPtr<UObjectTweenComponent> CloseDoorTween;

	UFUNCTION(BlueprintCallable)
	void DoorOpen();
	UFUNCTION(BlueprintCallable)
	void DoorClose();

	void Initialize(int32 InNormalCount, int32 InSpecialCount);

	void DecreaseNormal();
	void DecreaseSpecial();
	bool IsClear();

	virtual void BeginPlay() override;

protected:

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Blueprintable, Category = "Trigger")
	virtual void ExecuteTriggerAction(AActor* OtherActor);

private:
	UPROPERTY()
	TObjectPtr<UZoneChecker> ZoneChecker;

	UFUNCTION()
	void OpenDoor();
	UFUNCTION()
	void DoorUpdate(float InValue);
	UFUNCTION()
	void CloseDoor();
};

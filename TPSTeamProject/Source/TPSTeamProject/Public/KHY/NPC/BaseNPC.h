#pragma once

#include "CoreMinimal.h"
#include "KHY/Interactable/Interactable.h"
#include "GameFramework/Character.h"
#include "BaseNPC.generated.h"

class USphereComponent;
class USoundCue;

UCLASS()
class TPSTEAMPROJECT_API ABaseNPC : public ACharacter, public IInteractable
{
	GENERATED_BODY()

public:
	ABaseNPC();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sphere")
	USphereComponent* DetectionSphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Capsule")
	UCapsuleComponent* TagCapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimationAsset* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* Sound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Roatation")
	float RotationSpeed;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void LookCharacter(float DeltaTime);

	virtual void Interact(AActor* Interactor) override;

private:
	bool bExist;
	bool bInteract;
	AActor* Player;

};

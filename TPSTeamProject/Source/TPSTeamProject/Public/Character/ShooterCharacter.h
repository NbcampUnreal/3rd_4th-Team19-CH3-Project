// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Manager/ObserverManager/Observer.h"
#include "ShooterCharacter.generated.h"

struct FInputActionValue;
class UInventoryWidget;
class UInventoryComponent;
class USpringArmComponent;
class UCameraComponent;
class UObjectTweenComponent;
class UStatCalculater;
class USoundCue;

UCLASS()
class TPSTEAMPROJECT_API AShooterCharacter : public ACharacter, public IObserver
{
	GENERATED_BODY()

public:
	AShooterCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	TSubclassOf<class AGunActor> GunClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	TObjectPtr<class AGunActor> GunActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectTween")
	TObjectPtr<UObjectTweenComponent> ZoomTween;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> FireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundCue> DeathSound;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveValue")
	float InputDirectionAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectTween")
	bool bIsShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectTween")
	bool bIsAuto;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectTween")
	bool bIsZoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectTween")
	bool bIsCloseContact;
	UPROPERTY()
	TArray<TObjectPtr<UStatCalculater>> StatCalculaters;

	float Health = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComp;
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ToggleInventory();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void OnEvent(EMessageType InMsgType, int32 InParam) override;

	UFUNCTION()
	void OnDeath();
	UFUNCTION()
	void PlaySoundDeath();
	UFUNCTION()
	void PlayAnimationDeath();

private:

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Shooting(const FInputActionValue& value);
	UFUNCTION()
	void StopShooting(const FInputActionValue& value);
	UFUNCTION()
	void StartCloseContact(const FInputActionValue& value);
	UFUNCTION()
	void EndCloseContact(const FInputActionValue& value);
	UFUNCTION()
	void ZoomStart(const FInputActionValue& value);
	UFUNCTION()
	void ZoomEnd(const FInputActionValue& value);
	UFUNCTION()
	void TurnAuto(const FInputActionValue& value);
	UFUNCTION()
	void ZoomTimelineUpdate(float InValue);
	UFUNCTION()
	void ZoomTimelineFinished();
	UFUNCTION()
	void Interaction();
	UFUNCTION()
	void OnInventoryUpdated();

	FTimerDelegate ShootDelegate;
	FTimerDelegate CloseContactDelegate;

	FTimerHandle AutoShootTimerHandle;
	FTimerHandle CloseContactTimerHandle;
};

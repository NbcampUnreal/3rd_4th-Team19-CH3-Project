// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Manager/ObserverManager/Observer.h"
#include "GenericTeamAgentInterface.h" 
#include "ShooterCharacter.generated.h"

struct FInventorySlot;
struct FInputActionValue;
class UInventoryWidget;
class UInventoryComponent;
class USpringArmComponent;
class UCameraComponent;
class UObjectTweenComponent;
class UStatCalculater;
class USoundCue;
class UCrosshairComponent;

UCLASS()
class TPSTEAMPROJECT_API AShooterCharacter : public ACharacter, public IObserver, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AShooterCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	TSubclassOf<class AGunActor> GunClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
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
	TObjectPtr<UAnimMontage> ReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundCue> DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	TObjectPtr<UCrosshairComponent> CrosshairComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterState")
	bool bIsShoot;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveValue")
	float InputDirectionAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterState")
	bool bIsAuto;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterState")
	bool bIsZoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterState")
	bool bIsCloseContact;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterState")
	bool bIsReload;
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

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	UFUNCTION()
	void OnDeath();
	UFUNCTION()
	void PlaySoundDeath();
	UFUNCTION()
	void PlayAnimationDeath();

private:

	UPROPERTY()
	float SprintSpeed;
	float NormalSpeed;

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
	UFUNCTION()
	void Reload();
	UFUNCTION()
	void ReloadFinished();
	UFUNCTION()
	void OnSprint();
	UFUNCTION()
	void OffSprint();

	UFUNCTION()
	bool CanFire();
	UFUNCTION()
	void HandleItemUse(const FInventorySlot& SlotData);

	FTimerDelegate ShootDelegate;
	FTimerDelegate CloseContactDelegate;

	FTimerHandle AutoShootTimerHandle;
	FTimerHandle CloseContactTimerHandle;

	FGenericTeamId TeamId = FGenericTeamId(2);
};

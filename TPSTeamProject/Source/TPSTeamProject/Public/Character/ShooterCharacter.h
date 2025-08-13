// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UObjectTweenComponent;

UCLASS()
class TPSTEAMPROJECT_API AShooterCharacter : public ACharacter
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
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	TObjectPtr<UCurveFloat> ZoomCurveFloat;*/

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

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


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

	FTimerDelegate ShootDelegate;
	FTimerDelegate CloseContactDelegate;

	FTimerHandle AutoShootTimerHandle;
	FTimerHandle CloseContactTimerHandle;
};

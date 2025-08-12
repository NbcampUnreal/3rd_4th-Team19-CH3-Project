#include "Character/ShooterCharacter.h"
#include "Character/ShootPlayerController.h"
#include "Gun/GunActor.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Taeyeon/InventoryComponent.h"
#include "Util/Component/ObjectTweenComponent.h"

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	ZoomTween = CreateDefaultSubobject<UObjectTweenComponent>(TEXT("ZoomTween"));
	InventorySystem = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	bIsZoom = false;
	bIsAuto = false;
	bIsCloseContact = false;
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GunActor = GetWorld()->SpawnActor<AGunActor>(GunClass);
	GetMesh()->HideBoneByName(TEXT("Weapon"), EPhysBodyOp::PBO_None);
	GunActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon"));
	GunActor->SetOwner(this);

	ShootDelegate.BindLambda([this]()
	{
		if (IsValid(GunActor) == false)
		{
			return;
		}

		GunActor->Fire();
	});

	CloseContactDelegate.BindLambda([this]()
	{
		if (IsValid(SpringArmComp) == false)
		{
			return;
		}

		//ZoomTimeline->EndPlay(EEndPlayReason::Quit);
		bIsCloseContact = true;

		ZoomTween->PlayFromStart();
	});

	ZoomTween->AddFloatEvent(this, FName("ZoomTimelineUpdate"), FName("ZoomTimelineFinished"));

	/*if (ZoomCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("ZoomTimelineUpdate"));

		FOnTimelineEventStatic TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("ZoomTimelineFinished"));

		ZoomTimeline->AddInterpFloat(ZoomCurveFloat, TimelineProgress);
		ZoomTimeline->SetTimelineFinishedFunc(TimelineFinished);
	}*/
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AShootPlayerController* PlayerController = Cast<AShootPlayerController>(GetController()))
		{
			ensure(PlayerController->ShootAction);
			{
				EnhancedInput->BindAction(
					PlayerController->ShootAction,
					ETriggerEvent::Started,
					this,
					&AShooterCharacter::Shooting
				);

				EnhancedInput->BindAction(
					PlayerController->ShootAction,
					ETriggerEvent::Completed,
					this,
					&AShooterCharacter::StopShooting
				);
			}

			ensure(PlayerController->CloseContactAction);
			{
				EnhancedInput->BindAction(
					PlayerController->CloseContactAction,
					ETriggerEvent::Ongoing,
					this,
					&AShooterCharacter::StartCloseContact
				);

				EnhancedInput->BindAction(
					PlayerController->CloseContactAction,
					ETriggerEvent::Completed,
					this,
					&AShooterCharacter::EndCloseContact
				);
			}

			ensure(PlayerController->ZoomAction);
			{
				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Started,
					this,
					&AShooterCharacter::ZoomStart
				);

				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Completed,
					this,
					&AShooterCharacter::ZoomEnd
				);
			}

			ensure(PlayerController->TurnAutoAction);
			{
				EnhancedInput->BindAction(
					PlayerController->TurnAutoAction,
					ETriggerEvent::Started,
					this,
					&AShooterCharacter::TurnAuto
				);
			}
		}
	}
}

void AShooterCharacter::Shooting(const FInputActionValue& value)
{
	/*bool bShootTrigger = value.Get<bool>();
	if (bShootTrigger == false)
	{
		return;
	}*/

	if (bIsAuto == false)
	{
		if (bIsShoot == false)
		{
			bIsShoot = true;

			GunActor->Fire();
		}

		return;
	}

	bIsShoot = true;

	if (IsValid(GunActor) == false)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(
		AutoShootTimerHandle,
		ShootDelegate,
		0.2f,
		true
	);
}

void AShooterCharacter::StopShooting(const FInputActionValue& value)
{
	/*bool bShootTrigger = value.Get<bool>();
	if (bShootTrigger == false)
	{
		return;
	}*/

	bIsShoot = false;

	if (AutoShootTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AutoShootTimerHandle);
	}
}

void AShooterCharacter::StartCloseContact(const FInputActionValue& value)
{
	/*if (bIsZoom)
	{
		return;
	}*/

	SpringArmComp->TargetArmLength = 150.f;
}

void AShooterCharacter::EndCloseContact(const FInputActionValue& value)
{
	/*if (bIsZoom)
	{
		return;
	}*/

	SpringArmComp->TargetArmLength = 200.f;
}

void AShooterCharacter::ZoomStart(const FInputActionValue& value)
{
	if (bIsZoom == false)
	{
		GetWorldTimerManager().SetTimer(
			CloseContactTimerHandle,
			CloseContactDelegate,
			0.1f,
			false
		);
	}

	/*bIsZoom = !bIsZoom;

	if (bIsZoom)
	{
		AShootPlayerController* PlayerController = Cast<AShootPlayerController>(GetController());

		ensure(PlayerController);

		PlayerController->SetViewTargetWithBlend(GunActor, 0.4f);
	}
	else
	{
		AShootPlayerController* PlayerController = Cast<AShootPlayerController>(GetController());

		ensure(PlayerController);

		PlayerController->SetViewTargetWithBlend(this, 0.4f);
	}*/
}

void AShooterCharacter::ZoomEnd(const FInputActionValue& value)
{
	if (CloseContactTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(CloseContactTimerHandle);
	}

	if (bIsCloseContact)
	{
		//SpringArmComp->TargetArmLength = 200.f;

		//ZoomTimeline->EndPlay(EEndPlayReason::Quit);
		bIsCloseContact = false;

		ZoomTween->PlayFromStart();
	}
	else
	{
		if (bIsZoom == false)
		{
			AShootPlayerController* PlayerController = Cast<AShootPlayerController>(GetController());

			ensure(PlayerController);

			PlayerController->SetViewTargetWithBlend(GunActor, 0.4f);

			bIsZoom = true;
		}
		else
		{
			AShootPlayerController* PlayerController = Cast<AShootPlayerController>(GetController());

			ensure(PlayerController);

			PlayerController->SetViewTargetWithBlend(this, 0.4f);

			bIsZoom = false;
		}
	}
}

void AShooterCharacter::TurnAuto(const FInputActionValue& value)
{
	/*bool bTurnAuto = value.Get<bool>();
	if (bTurnAuto == false)
	{
		return;
	}*/

	bIsAuto = !bIsAuto;

	UE_LOG(LogTemp, Log, TEXT("bIsAuto : %s"), bIsAuto ? *FString(TEXT("True")) : *FString(TEXT("False")));
}

void AShooterCharacter::ZoomTimelineUpdate(float InValue)
{
	if (bIsCloseContact)
	{
		SpringArmComp->TargetArmLength
			= FMath::Clamp(FMath::Lerp(SpringArmComp->TargetArmLength, 100.f, InValue), 100.f, 200.f);
	}
	else
	{
		SpringArmComp->TargetArmLength
			= FMath::Clamp(FMath::Lerp(SpringArmComp->TargetArmLength, 200.f, InValue), 100.f, 200.f);
	}
}

void AShooterCharacter::ZoomTimelineFinished()
{
	if (bIsCloseContact)
	{
		SpringArmComp->TargetArmLength = 100.f;
	}
	else
	{
		SpringArmComp->TargetArmLength = 200.f;
	}
}


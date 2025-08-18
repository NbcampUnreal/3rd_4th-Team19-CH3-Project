#include "Character/ShooterCharacter.h"
#include "Character/ShootPlayerController.h"
#include "Gun/GunActor.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Taeyeon/InventoryComponent.h"
#include "Util/Component/ObjectTweenComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KHY/Interactable/Interactable.h"
#include "Taeyeon/InventoryWidget.h"
#include "Taeyeon/ItemComponent.h"
#include "Stat/StatCalculater.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/ObserverManager.h"
#include "Manager/ObserverManager/MessageType.h"

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
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

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

	if (UWorld* World = GetWorld())
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UObserverManager* ObserverManager = GameInstance->GetSubsystem<UObserverManager>(ESubsystemType::Observer);

		ObserverManager->Subscribe(this);
	}

	StatCalculaters.Add(GunActor->GetStatCalculater());

	ShootDelegate.BindLambda([this]()
	{
		if (IsValid(GunActor) == false)
		{
			return;
		}

		GunActor->Fire();

		double RecoilYaw = FMath::FRandRange(-0.1f, 0.1f);
		double RecoilPitch = FMath::FRandRange(-0.5f, 0.f);

		AddControllerYawInput(RecoilYaw);
		AddControllerPitchInput(RecoilPitch);

		if (FireMontage)
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(FireMontage);
			}
		}
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

	InventoryComp->OnInventoryUpdated.AddDynamic(this, &AShooterCharacter::OnInventoryUpdated);
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	InventoryWidget->OnItemUseRequested.AddDynamic(this, &AShooterCharacter::HandleItemUse);
}

void AShooterCharacter::ToggleInventory()
{
	check(InventoryWidgetClass);

	AShootPlayerController* PlayerController = Cast<AShootPlayerController>(GetController());
	check(PlayerController)
	{
		if (InventoryWidget->IsInViewport())
		{
			InventoryWidget->RemoveFromParent();
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetShowMouseCursor(false);
		}
		else
		{
			InventoryWidget->AddToViewport();
			PlayerController->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(InventoryWidget->TakeWidget()));
			PlayerController->SetShowMouseCursor(true);
			InventoryWidget->SetKeyboardFocus();
			InventoryWidget->RefreshInventory(InventoryComp);
		}
	}
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
			ensure(PlayerController->MoveAction);
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::Move
				);
			}

			ensure(PlayerController->LookAction);
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::Look
				);
			}

			ensure(PlayerController->JumpAction);
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AShooterCharacter::StopJump
				);
			}

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

			ensure(PlayerController->InteractionAction);
			{
				EnhancedInput->BindAction(
					PlayerController->InteractionAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::Interaction
				);
			}

			ensure(PlayerController->InventoryToggleAction);
			{
				EnhancedInput->BindAction(
					PlayerController->InventoryToggleAction,
					ETriggerEvent::Completed,
					this,
					&AShooterCharacter::ToggleInventory
				);
			}
		}
	}
}

void AShooterCharacter::Move(const FInputActionValue& value)
{
	const FVector Direction = value.Get<FVector>();
	/*if (Direction.IsNearlyZero(0.f))
	{
		return;
	}*/

	if (FMath::IsNearlyZero(Direction.X) == false)
	{
		AddMovementInput(GetActorForwardVector() * Direction.X, 5.f);
	}

	if (FMath::IsNearlyZero(Direction.Y) == false)
	{
		AddMovementInput(GetActorRightVector() * Direction.Y, 5.f);
	}
}

void AShooterCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void AShooterCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void AShooterCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
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

			double RecoilYaw = FMath::FRandRange(-0.1f, 0.1f);
			double RecoilPitch = FMath::FRandRange(-0.5f, 0.f);

			AddControllerYawInput(RecoilYaw);
			AddControllerPitchInput(RecoilPitch);

			if (FireMontage)
			{
				if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
				{
					AnimInstance->Montage_Play(FireMontage);
				}
			}
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
		0.1f,
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

			PlayerController->SetViewTargetWithBlend(GunActor, 0.1f);

			bIsZoom = true;
		}
		else
		{
			AShootPlayerController* PlayerController = Cast<AShootPlayerController>(GetController());

			ensure(PlayerController);

			PlayerController->SetViewTargetWithBlend(this, 0.1f);

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

void AShooterCharacter::Interaction()
{
	TArray<FHitResult> OutHits;
	FVector MyLocation = GetActorLocation();

	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		MyLocation,
		MyLocation,
		200.f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{this},
		EDrawDebugTrace::ForDuration,
		OutHits,
		true
	);

	if (bHit)
	{
		for (const auto& Hit : OutHits)
		{
			UPrimitiveComponent* HitComponent = Hit.GetComponent();
			if (HitComponent)
			{
				if (HitComponent && HitComponent->ComponentTags.Contains(TEXT("Interaction")))
				{
					AActor* HitActor = Hit.GetActor();
					UItemComponent* ItemComponent = HitActor->FindComponentByClass<UItemComponent>();
					IInteractable* InteractableActor = Cast<IInteractable>(ItemComponent);

					if (InteractableActor && HitActor)
					{
						//IInteractable::Execute(HitActor, this);
						InteractableActor->Interact(this);
						break;
					}
				}
			}
		}
	}
}

void AShooterCharacter::OnInventoryUpdated()
{
	if (InventoryWidget && InventoryWidget->IsInViewport())
	{
		InventoryWidget->RefreshInventory(InventoryComp);
	}
}

void AShooterCharacter::HandleItemUse(const FInventorySlot& SlotData)
{
	if (SlotData.ItemName.IsNone())
	{
		return;
	}

	InventoryComp->RemoveItem(SlotData.ItemName, 1);
}

float AShooterCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= ActualDamage;
	UE_LOG(LogTemp, Warning, TEXT("[Player] took %.1f damage. Current HP: %.1f"), ActualDamage, Health);

	if (Health <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is dead."));
		OnDeath();
	}

	return ActualDamage;
}

void AShooterCharacter::OnEvent(EMessageType InMsgType, int32 InParam)
{
	if (InMsgType == EMessageType::UpdateStat)
	{
		int32 MaxHealth = 0;
		for (const auto StatCalculater : StatCalculaters)
		{
			MaxHealth += StatCalculater->GetMaxHP();
		}

		Health = MaxHealth;
	}
}

void AShooterCharacter::OnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Death!"));
}


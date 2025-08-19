#include "Enemy/EnemyCharacter.h"
#include "Enemy/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "Taeyeon/Item.h"
#include "Taeyeon/ItemComponent.h"
#include "Enemy/EnemyHPBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Manager/GameInstanceSubsystem/ObserverManager.h"
#include "Manager/ObserverManager/MessageType.h"

AEnemyCharacter::AEnemyCharacter()
{
	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	Movement->AirControl = 0.2f;

	BodyCollision = GetCapsuleComponent();
	BodyCollision->SetGenerateOverlapEvents(false);

	RightArmCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightArmCollision"));
	RightArmCollision->SetupAttachment(GetMesh(), TEXT("hand_r"));
	RightArmCollision->SetGenerateOverlapEvents(false);

	LeftArmCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftArmCollision"));
	LeftArmCollision->SetupAttachment(GetMesh(), TEXT("hand_l"));
	LeftArmCollision->SetGenerateOverlapEvents(false);

	WalkerHPbarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WalkerHPbarWidgetComponent"));
	WalkerHPbarWidgetComponent->SetupAttachment(GetMesh());
	WalkerHPbarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WalkerHPbarWidgetComponent->SetVisibility(false);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UDataTableManager* DataTableMgr = GameInstance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);
		FEnemyStatDataStruct* WalkerData = DataTableMgr->GetData<FEnemyStatDataStruct>(EDataType::EnemyStat, static_cast<int32>(EnemyType));

		if (WalkerData)
		{
			MaxHealth = WalkerData->DefaultHP;
			Health = WalkerData->DefaultHP;
			Damage = WalkerData->DefaultAtk;
			WalkSpeed = WalkerData->WalkSpeed;
			RunSpeed = WalkerData->RunSpeed;
		}
	}

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed;

	BaseDamage = Damage;

	AEnemyController* AIController = Cast<AEnemyController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), false);
	}

	RightArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnArmOverlap);
	LeftArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnArmOverlap);

	//if (EnemyType != EEnemyType::Walker)
	//{
	//	ShowBossHPbar();
	//}
}

float AEnemyCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= ActualDamage;
	UE_LOG(LogTemp, Warning, TEXT("[Enemy] took %.1f damage. Current HP: %.1f"), ActualDamage, Health);

	if (EnemyType == EEnemyType::Walker)
	{
		if (!WalkerHPbarWidgetComponent->IsVisible())
		{
			ShowWalkerHPbar();
		}
		UpdateWalkerHPbar();
	}
	else
	{
		if (!BossHPbarWidgetInstance)
		{
			ShowBossHPbar();
		}
		UpdateBossHPbar();
	}

	if (Health <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy is dead."));
		OnDeath();
	}

	if (ActualDamage > 0.0f)
	{
		AEnemyController* AIController = Cast<AEnemyController>(GetController());
		if (AIController && EventInstigator)
		{
			APawn* InstigatorPawn = EventInstigator->GetPawn();
			if (InstigatorPawn)
			{
				FVector HitLocation = InstigatorPawn->GetActorLocation();
				AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("WasDamaged"), true);
				AIController->GetBlackboardComponent()->SetValueAsVector(TEXT("HitLocation"), HitLocation);
			}
		}
	}

	return ActualDamage;
}

void AEnemyCharacter::ResetDamage()
{
	Damage = BaseDamage;
}

void AEnemyCharacter::MultiplyDamage(float Multiplier)
{
	Damage = BaseDamage * Multiplier;
}

void AEnemyCharacter::UpdateMovementSpeed()
{
	AEnemyController* AIController = Cast<AEnemyController>(GetController());
	if (!AIController) return;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (!Movement) return;

	bool bTaskRunning = AIController->GetBlackboardComponent()->GetValueAsBool(TEXT("TaskRunning"));
	bool bCanSeeTarget = AIController->GetBlackboardComponent()->GetValueAsBool(TEXT("CanSeeTarget"));

	if (bTaskRunning || bCanSeeTarget)
		Movement->MaxWalkSpeed = RunSpeed;
	else
		Movement->MaxWalkSpeed = WalkSpeed;
}

void AEnemyCharacter::EnableRightArmCollision(bool bEnable)
{
	RightArmCollision->SetGenerateOverlapEvents(bEnable);
}

void AEnemyCharacter::EnableBothArmCollision(bool bEnable)
{
	RightArmCollision->SetGenerateOverlapEvents(bEnable);
	LeftArmCollision->SetGenerateOverlapEvents(bEnable);
}

void AEnemyCharacter::OnArmOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (bHasGivenDamage) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActor && OtherActor == PlayerPawn)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			GetInstigatorController(),
			this,
			nullptr
		);

		bHasGivenDamage = true;
	}
}

void AEnemyCharacter::OnDeath()
{
	if (EnemyType == EEnemyType::Walker)
	{
		HideWalkerHPbar();
	}
	else
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}

		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UObserverManager* ObserverManager = GameInstance->GetSubsystem<UObserverManager>(ESubsystemType::Observer);

		if (EnemyType == EEnemyType::Screamer)
		{
			ObserverManager->SendEvent(EMessageType::KillSpecial, 1);
		}
		else if (EnemyType == EEnemyType::Cloaker)
		{
			ObserverManager->SendEvent(EMessageType::KillSpecial, 2);
		}
		else if (EnemyType == EEnemyType::Spitter)
		{
			ObserverManager->SendEvent(EMessageType::KillSpecial, 3);
		}
		else if (EnemyType == EEnemyType::Brute)
		{
			ObserverManager->SendEvent(EMessageType::KillSpecial, 4);
		}

		HideBossHPbar();
	}

	AEnemyController* AIController = Cast<AEnemyController>(GetController());
	if (AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsDead?"));
		AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), true);
		AIController->BrainComponent->StopLogic("Dead");
		AIController->UnPossess();
	}

	// 충돌 비활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DropItems();
	
	if (DeathMontage && GetMesh())
	{
		float Duration = PlayAnimMontage(DeathMontage);
		if (Duration > 0.0f)
		{
			GetWorldTimerManager().SetTimer(
				DeathTimerHandle,
				this,
				&AEnemyCharacter::OnDeathAnimationFinished,
				Duration -0.2f,
				false
			);
		}
		else
		{
			Destroy();
		}
	}
	else
	{
		Destroy();
	}
}
void AEnemyCharacter::OnDeathAnimationFinished()
{
	Destroy();
}

void AEnemyCharacter::DropItems()
{
	if (EnemyType == EEnemyType::Walker || (DropCoin == nullptr))
	{
		return;
	}

	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();
	
	if (DropCoin)
	{
		GetWorld()->SpawnActor<AItem>(DropCoin,
			FVector(SpawnLocation.X, SpawnLocation.Y,(SpawnLocation.Z - 90.f)),
			SpawnRotation);
	}

	DropAttachment();
}

void AEnemyCharacter::DropAttachment()
{
	if (!AttachmentLootTable || !CommonAttachmentItemClass)
	{
		return;
	}

	if (FMath::FRand() < AttachmentDropChance)
	{
		const TArray<FName> RowNames = AttachmentLootTable->GetRowNames();
		if (RowNames.IsEmpty())
		{
			return;
		}

		const int32 DropCount = FMath::RandRange(MinAttachmentDrop, MaxAttachmentDrop);

		for (int32 i = 0; i < DropCount; ++i)
		{
			const FName RandomAttachmentName = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];

			FVector SpawnLocation = GetActorLocation();
			FHitResult HitResult;
			FVector StartLocation = GetActorLocation();
			FVector EndLocation = StartLocation - FVector(0.f, 0.f, 500.f);
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility,
			                                         CollisionParams))
			{
				SpawnLocation = HitResult.Location;
			}

			float RandomX = FMath::FRandRange(-ItemSpawnRadius, ItemSpawnRadius);
			float RandomY = FMath::FRandRange(-ItemSpawnRadius, ItemSpawnRadius);
			FVector RandomOffset = FVector(RandomX, RandomY, 0.f);
			SpawnLocation += RandomOffset;

			if (AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(CommonAttachmentItemClass, SpawnLocation,
			                                                       FRotator::ZeroRotator))
			{
				if (UItemComponent* ItemComp = SpawnedItem->FindComponentByClass<UItemComponent>())
				{
					ItemComp->SetItemName(RandomAttachmentName);
				}
			}
		}
	}
}

void AEnemyCharacter::ShowBossHPbar()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return;

	if (BossHPbarWidgetInstance)
	{
		HideBossHPbar();
	}

	BossHPbarWidgetInstance = CreateWidget<UEnemyHPBarWidget>(PlayerController, BossHPbarWidgetClass);

	if (BossHPbarWidgetInstance)
	{
		BossHPbarWidgetInstance->AddToViewport();
		BossHPbarWidgetInstance->InitEnemy(this);
	}
}

void AEnemyCharacter::UpdateBossHPbar()
{
	if (BossHPbarWidgetInstance)
	{
		BossHPbarWidgetInstance->UpdateHP(Health, MaxHealth);
	}
}

void AEnemyCharacter::HideBossHPbar()
{
	if (BossHPbarWidgetInstance)
	{
		BossHPbarWidgetInstance->RemoveFromParent();
		BossHPbarWidgetInstance = nullptr;
	}
}

void AEnemyCharacter::ShowWalkerHPbar()
{
	if (!WalkerHPbarWidgetClass && !WalkerHPbarWidgetComponent) return;

	WalkerHPbarWidgetComponent->SetWidgetClass(WalkerHPbarWidgetClass);
	WalkerHPbarWidgetComponent->SetVisibility(true);
	if (UUserWidget* Widget = WalkerHPbarWidgetComponent->GetUserWidgetObject())
	{
		if (UEnemyHPBarWidget* WalkerWidget = Cast<UEnemyHPBarWidget>(Widget))
		{
			WalkerWidget->InitEnemy(this);
		}
	}
}

void AEnemyCharacter::UpdateWalkerHPbar()
{
	if (WalkerHPbarWidgetComponent && WalkerHPbarWidgetComponent->GetUserWidgetObject())
	{
		if (UEnemyHPBarWidget* WalkerWidget = Cast<UEnemyHPBarWidget>(WalkerHPbarWidgetComponent->GetUserWidgetObject()))
		{
			WalkerWidget->UpdateHP(Health, MaxHealth);
		}
	}
}

void AEnemyCharacter::HideWalkerHPbar()
{
	if (WalkerHPbarWidgetComponent)
	{
		WalkerHPbarWidgetComponent->SetVisibility(false);
	}
}



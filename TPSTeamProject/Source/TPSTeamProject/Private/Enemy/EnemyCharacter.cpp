#include "Enemy/EnemyCharacter.h"
#include "Enemy/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter()
{
	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	Movement->AirControl = 0.2f;

	RightArmCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightArmCollision"));
	RightArmCollision->SetupAttachment(GetMesh(), TEXT("hand_r"));
	RightArmCollision->SetGenerateOverlapEvents(false);

	LeftArmCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftArmCollision"));
	LeftArmCollision->SetupAttachment(GetMesh(), TEXT("hand_l"));
	LeftArmCollision->SetGenerateOverlapEvents(false);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AEnemyController* AIController = Cast<AEnemyController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), false);
	}

	RightArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnArmOverlap);
	LeftArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnArmOverlap);
}

float AEnemyCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= ActualDamage;
	UE_LOG(LogTemp, Warning, TEXT("Player took %.1f damage. Current HP: %.1f"), ActualDamage, Health);

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
				AIController->GetBlackboardComponent()->SetValueAsVector(TEXT("DamagedLocation"), HitLocation);
			}
		}
	}

	return ActualDamage;
}

void AEnemyCharacter::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
	}
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
	AEnemyController* AIController = Cast<AEnemyController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), true);
		AIController->UnPossess();
	}

	// 충돌 비활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (DeathMontage && GetMesh())
	{
		float Duration = PlayAnimMontage(DeathMontage);

		GetWorldTimerManager().SetTimer(
			DeathTimerHandle,
			this,
			&AEnemyCharacter::OnDeathAnimationFinished,
			Duration,
			false
		);
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



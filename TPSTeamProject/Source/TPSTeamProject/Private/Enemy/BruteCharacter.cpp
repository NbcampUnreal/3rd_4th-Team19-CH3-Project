#include "Enemy/BruteCharacter.h"
#include "Enemy/BruteController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ABruteCharacter::ABruteCharacter()
{
	AIControllerClass = ABruteController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyType = EEnemyType::Brute;

	RushCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RushCollision"));
	RushCollision->SetupAttachment(BodyCollision);
	RushCollision->SetCapsuleHalfHeight(BodyCollision->GetUnscaledCapsuleHalfHeight() * 2.0f);
	RushCollision->SetCapsuleRadius(BodyCollision->GetUnscaledCapsuleRadius() * 2.0f);
	RushCollision->SetGenerateOverlapEvents(false);

	SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
}

void ABruteCharacter::BeginPlay()
{
	Super::BeginPlay();

	RushCollision->OnComponentBeginOverlap.AddDynamic(this, &ABruteCharacter::OnCapsuleOverlap);
}

void ABruteCharacter::EnableRushCollision(bool bEnable)
{
	RushCollision->SetGenerateOverlapEvents(bEnable);
}

void ABruteCharacter::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	TArray<AActor*> OverlappingActors;
	OverlappedComponent->GetOverlappingActors(OverlappingActors);

	UE_LOG(LogTemp, Warning, TEXT("=== Overlapping Actors (%d) ==="), OverlappingActors.Num());

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || Actor == this) continue;

		UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s, Class: %s"),
			*Actor->GetName(),
			*Actor->GetClass()->GetName()
		);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Actor == PlayerPawn)
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				Damage,
				GetInstigatorController(),
				this,
				nullptr
			);

			UE_LOG(LogTemp, Warning, TEXT("Damaged"));
		}

		ACharacter* OtherChar = Cast<ACharacter>(Actor);
		if (OtherChar)
		{
			UE_LOG(LogTemp, Warning, TEXT("Character IS"));

			UCharacterMovementComponent* OtherCharMovement = OtherChar->FindComponentByClass<UCharacterMovementComponent>();
			if (OtherCharMovement)
			{
				FVector LaunchDirection = OtherChar->GetActorLocation() - GetActorLocation();
				LaunchDirection.Z = 0.5f;
				LaunchDirection.Normalize();

				float LaunchStrength = 2000.0f;
				OtherChar->LaunchCharacter(
					FVector(LaunchDirection.X * LaunchStrength, LaunchDirection.Y * LaunchStrength, LaunchStrength),
					true,
					true
				);
			}
		}
	}
}
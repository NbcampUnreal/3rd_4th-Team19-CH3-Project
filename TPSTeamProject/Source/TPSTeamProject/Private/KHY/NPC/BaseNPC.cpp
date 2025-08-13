#include "KHY/NPC/BaseNPC.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimationAsset.h"
#include "Sound/SoundCue.h"


ABaseNPC::ABaseNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	DetectionSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphereComp->SetupAttachment(RootComponent);
	DetectionSphereComp->InitSphereRadius(250.0f);

	DetectionSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseNPC::OnOverlapBegin);
	DetectionSphereComp->OnComponentEndOverlap.AddDynamic(this, &ABaseNPC::OnOverlapEnd);


	TagCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	TagCapsuleComp->SetupAttachment(RootComponent);


	bExist = false;
	bInteract = false;

	RotationSpeed = 5.0f;

	Player = nullptr;
}

void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bExist)
	{
		LookCharacter(DeltaTime);
	}

}

void ABaseNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(ACharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Position"), *OtherActor->GetName());
			bExist = true;
			Player = OtherActor;

			if (Sound)
			{
				if (Sound->IsPlayable())
				{
					UGameplayStatics::PlaySound2D(this, Sound);
				}
			}

			if (Animation)
			{
				GetMesh()->PlayAnimation(Animation, false);
			}
		}
	}
}

void ABaseNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		if (OtherActor->IsA(ACharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Leave"), *OtherActor->GetName());
			if (Player == OtherActor)
			{
				bExist = false;
				Player = nullptr;
			}
		}
	}
}

void ABaseNPC::LookCharacter(float DeltaTime)
{
	if (Player)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		FVector NPCLocation = GetActorLocation();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(NPCLocation, PlayerLocation);
		TargetRotation.Pitch = 0.0f;
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);


		SetActorRotation(NewRotation);
	}
}

void ABaseNPC::Interact(AActor* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("NPC"));
}


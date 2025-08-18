#include "KHY/NPC/BaseNPC.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimationAsset.h"
#include "Sound/SoundCue.h"
#include "Util/Component/ObjectTweenComponent.h"


ABaseNPC::ABaseNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	DetectionSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphereComp->SetupAttachment(RootComponent);
	DetectionSphereComp->InitSphereRadius(250.0f);

	DetectionSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseNPC::OnOverlapBegin);
	DetectionSphereComp->OnComponentEndOverlap.AddDynamic(this, &ABaseNPC::OnOverlapEnd);


	TagCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	TagCapsuleComp->SetupAttachment(RootComponent);

	LookTween = CreateDefaultSubobject<UObjectTweenComponent>(TEXT("LookTween"));

	bExist = false;
	bInteract = false;

	RotationSpeed = 5.0f;

	Player = nullptr;
}

void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();

	LookDelegate.BindLambda([this]()
	{
		if (IsValid(Player) == false && bExist)
		{
			return;
		}

		LookTween->PlayFromStart();
	});

	LookTween->AddFloatEvent(this, FName("LookCharacter"), FName(NAME_None));
}


void ABaseNPC::PlaySound()
{
	if (!Sound)	return;

	if (Sound->IsPlayable())
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
}

void ABaseNPC::PlayAnimation()
{
	if (!Animation)	return;	

	GetMesh()->PlayAnimation(Animation, false);
}


void ABaseNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(ACharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Position"), *OtherActor->GetName());
			bExist = true;
			Player = Cast<ACharacter>(OtherActor);

			PlaySound();
			PlayAnimation();	
			LookStart();
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
				LookEnd();
			}
		}
	}
}

void ABaseNPC::LookCharacter(float InValue)
{
	if (Player)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		FVector NPCLocation = GetActorLocation();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(NPCLocation, PlayerLocation);
		TargetRotation.Pitch = 0.0f;
		FRotator CurrentRotation = GetActorRotation();

		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, InValue, RotationSpeed);


		SetActorRotation(NewRotation);
	}
}

void ABaseNPC::LookStart()
{
	GetWorldTimerManager().SetTimer(
		LookTimerHandle,
		LookDelegate,
		0.1f,
		true
	);
}

void ABaseNPC::LookEnd()
{
	if (LookTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(LookTimerHandle);
	}
}

void ABaseNPC::Interact(AActor* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("NPC"));
}


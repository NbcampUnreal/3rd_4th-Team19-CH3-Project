#include "KHY/Zone/BaseZone.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameMode/ZoneChecker/ZoneChecker.h"
#include "Util/Component/ObjectTweenComponent.h"

ABaseZone::ABaseZone()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseZone::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABaseZone::OnOverlapEnd);

	OpenDoorTween = CreateDefaultSubobject<UObjectTweenComponent>(TEXT("OpenDoorTween"));
	CloseDoorTween = CreateDefaultSubobject<UObjectTweenComponent>(TEXT("CloseDoorTween"));

	ZoneChecker = NewObject<UZoneChecker>();
}

void ABaseZone::DoorOpen()
{
	OpenDoorTween->PlayFromStart();
}

void ABaseZone::DoorClose()
{
	CloseDoorTween->PlayFromStart();
}

void ABaseZone::Initialize(int32 InNormalCount, int32 InSpecialCount)
{
	ZoneChecker->Initialize(InNormalCount, InSpecialCount);
}

void ABaseZone::DecreaseNormal()
{
	ZoneChecker->DecreaseNormalMonsterCount();
}

void ABaseZone::DecreaseSpecial()
{
	ZoneChecker->DecreaseSpecialMonsterCount();
}

bool ABaseZone::IsClear()
{
	return ZoneChecker->IsClear();
}

void ABaseZone::BeginPlay()
{
	Super::BeginPlay();

	OpenDoorTween->AddFloatEvent(this, FName(TEXT("DoorUpdate")), FName(TEXT("OpenDoor")));
	CloseDoorTween->AddFloatEvent(this, FName(TEXT("DoorUpdate")), FName(TEXT("CloseDoor")));
}


void ABaseZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(ACharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Position"), *OtherActor->GetName());
			ExecuteTriggerAction(OtherActor);
		}

		if (ZoneChecker && ZoneChecker->IsClear() == false)
		{
			CloseDoor();
		}
	}
}

void ABaseZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		if (OtherActor->IsA(ACharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Leave"), *OtherActor->GetName());
		}
	}
}

void ABaseZone::ExecuteTriggerAction(AActor* OtherActor)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
	}

}

void ABaseZone::OpenDoor()
{
	FRotator CurrentDoorRatator = DoorMeshActor->GetActorRotation();

	float DoorRoll = CurrentDoorRatator.Roll;
	float DoorPitch = CurrentDoorRatator.Pitch;

	DoorMeshActor->SetActorRelativeRotation(FRotator(DoorRoll, DoorPitch, 90.f));
}

void ABaseZone::DoorUpdate(float InValue)
{
	FRotator CurrentDoorRatator = DoorMeshActor->GetActorRotation();

	float DoorRoll = CurrentDoorRatator.Roll;
	float DoorPitch = CurrentDoorRatator.Pitch;

	DoorMeshActor->SetActorRelativeRotation(FRotator(DoorRoll, DoorPitch, InValue));
}

void ABaseZone::CloseDoor()
{
	FRotator CurrentDoorRatator = DoorMeshActor->GetActorRotation();

	float DoorRoll = CurrentDoorRatator.Roll;
	float DoorPitch = CurrentDoorRatator.Pitch;

	DoorMeshActor->SetActorRelativeRotation(FRotator(DoorRoll, DoorPitch, 0.f));
}


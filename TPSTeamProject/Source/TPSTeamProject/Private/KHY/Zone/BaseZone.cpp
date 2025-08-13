#include "KHY/Zone/BaseZone.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

ABaseZone::ABaseZone()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseZone::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABaseZone::OnOverlapEnd);
}

void ABaseZone::BeginPlay()
{
	Super::BeginPlay();
	
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
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
}



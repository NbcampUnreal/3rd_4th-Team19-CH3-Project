#include "Enemy/CloakerCharacter.h"
#include "Enemy/CloakerController.h"
#include "Kismet/GameplayStatics.h"

ACloakerCharacter::ACloakerCharacter()
{
	AIControllerClass = ACloakerCharacter::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyType = EEnemyType::Cloaker;
}

void ACloakerCharacter::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (MeshComp)
	{
		OriginalMaterials.Empty();
		for (int32 i = 0; i < MeshComp->GetNumMaterials(); i++)
		{
			OriginalMaterials.Add(MeshComp->GetMaterial(i));
		}
	}

	SetTranslucent(true);
}

float ACloakerCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	bTakedDamage = true;
	SetTranslucent(false);

	return ActualDamage;
}

void ACloakerCharacter::OnArmOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActor && OtherActor == PlayerPawn)
	{
		FVector DirectionFromPlayer = GetActorLocation() - OtherActor->GetActorLocation();
		DirectionFromPlayer.Normalize();

		FVector PlayerForwardVector = OtherActor->GetActorForwardVector();
		PlayerForwardVector.Normalize();

		float DotProduct = FVector::DotProduct(DirectionFromPlayer, PlayerForwardVector);
		float Threshold = -0.5f;

		ResetDamage();

		if (DotProduct < Threshold)
		{
			MultiplyDamage(2.0f);
		}
	}

	Super::OnArmOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ACloakerCharacter::SetTranslucent(bool bTransluncent)
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	UE_LOG(LogTemp, Warning, TEXT("OriginalMaterials.Num()=%d, Mesh.NumMaterials()=%d"),
		OriginalMaterials.Num(), MeshComp->GetNumMaterials());

	if (bTransluncent && TranslucentMaterial && !bTakedDamage)
	{
		for (int32 i = 0; i < MeshComp->GetNumMaterials(); i++)
		{
			MeshComp->SetMaterial(i, TranslucentMaterial);
		}
	}
	else
	{
		for (int32 i = 0; i < OriginalMaterials.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Slot %d: %s"), i, *OriginalMaterials[i]->GetName());
			MeshComp->SetMaterial(i, OriginalMaterials[i]);
		}
	}
}

#include "Enemy/SpitterCharacter.h"
#include "Enemy/SpitterController.h"
#include "Engine/SkeletalMeshSocket.h"

ASpitterCharacter::ASpitterCharacter()
{
	AIControllerClass = ASpitterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyType = EEnemyType::Spitter;
}

void ASpitterCharacter::SpitAcid()
{
	UE_LOG(LogTemp, Warning, TEXT("SpitAcid called"));

	if (!SpitProjectileClass) return;

	UE_LOG(LogTemp, Warning, TEXT("SpitProjectileClass Exists"));

	FVector SpawnLoc = GetActorLocation();
	FRotator SpawnRot = GetActorRotation();

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (SpitSocketName != NAME_None)
		{
			if (const USkeletalMeshSocket* Socket = MeshComp->GetSocketByName(SpitSocketName))
			{
				SpawnLoc = Socket->GetSocketLocation(MeshComp);
			}
		}
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAcidProjecfile* Projectile = GetWorld()->SpawnActor<AAcidProjecfile>(SpitProjectileClass, SpawnLoc, SpawnRot, SpawnParams);
	if (Projectile)
	{
		Projectile->DamageAmount = this->Damage;
	}
}

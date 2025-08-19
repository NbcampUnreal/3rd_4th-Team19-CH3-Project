#include "Enemy/ScreamerCharacter.h"
#include "Enemy/ScreamerController.h"
#include "NavigationSystem.h"

AScreamerCharacter::AScreamerCharacter()
{
	AIControllerClass = AScreamerController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyType = EEnemyType::Screamer;
}

void AScreamerCharacter::Scream()
{
	if (!EnemyToSpawnClass) return;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return;

	FVector ScreamerLocation = GetActorLocation();

	int32 LoopCount = SpawnCount;
	int32 MaxAttempts = SpawnCount * 10;

	while(LoopCount>0 && MaxAttempts>0)
	{
		FNavLocation RandomLocation;
		bool bFound = NavSystem->GetRandomReachablePointInRadius(
			ScreamerLocation,
			SpawnRadius,
			RandomLocation
		);

		if (bFound)
		{
			FVector SpawnLoc = RandomLocation.Location;

			FVector DirectionToScreamer = (ScreamerLocation - SpawnLoc).GetSafeNormal();
			FRotator SpawnRot = DirectionToScreamer.Rotation();

			AWalkerCharacter* SpawnedActor = GetWorld()->SpawnActor<AWalkerCharacter>(EnemyToSpawnClass, SpawnLoc, SpawnRot);
			if (SpawnedActor)
			{
				LoopCount--;
			}
		}

		MaxAttempts--;
	}
}

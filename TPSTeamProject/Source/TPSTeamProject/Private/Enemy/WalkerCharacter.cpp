#include "Enemy/WalkerCharacter.h"
#include "Enemy/WalkerController.h"

AWalkerCharacter::AWalkerCharacter()
{
	AIControllerClass = AWalkerController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyType = EEnemyType::Walker;
}


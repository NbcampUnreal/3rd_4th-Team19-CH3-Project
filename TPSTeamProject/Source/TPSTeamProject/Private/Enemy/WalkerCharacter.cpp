#include "Enemy/WalkerCharacter.h"
#include "Enemy/WalkerController.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "GameData/EnemyStatDataStruct.h"
#include "GameFramework/CharacterMovementComponent.h"

AWalkerCharacter::AWalkerCharacter()
{
	AIControllerClass = AWalkerController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AWalkerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (World)
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UDataTableManager* DataTableMgr = GameInstance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);
		FEnemyStatDataStruct* WalkerData = DataTableMgr->GetData<FEnemyStatDataStruct>(EDataType::EnemyStat, 1);

		if (WalkerData)
		{
			Health = WalkerData->DefaultHP;
			Damage = WalkerData->DefaultAtk;
			WalkSpeed = WalkerData->WalkSpeed;
			RunSpeed = WalkerData->RunSpeed;
		}
	}

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed;
}


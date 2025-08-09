#include "Enemy/WalkerController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "GameData/EnemyStatDataStruct.h"

AWalkerController::AWalkerController()
{

}

void AWalkerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UWorld* World = GetWorld();
	if (World)
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UDataTableManager* DataTableMgr = GameInstance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);
		FEnemyStatDataStruct* WalkerData = DataTableMgr->GetData<FEnemyStatDataStruct>(EDataType::EnemyStat, 1);

		if (WalkerData)
		{
			SightConfig->SightRadius = WalkerData->SightRadius;
			SightConfig->LoseSightRadius = WalkerData->LoseSightRadius;
			SightConfig->PeripheralVisionAngleDegrees = WalkerData->PeripheralVisionAngleDegress;
			SightConfig->SetMaxAge(WalkerData->SightMaxAge);

			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			SightConfig->DetectionByAffiliation.bDetectNeutrals= true;  //팀 ID 설정후 삭제
			SightConfig->DetectionByAffiliation.bDetectFriendlies = true; //팀 ID 설정 후 삭제

			HearingConfig->HearingRange = WalkerData->HearingRange;
			HearingConfig->LoSHearingRange = WalkerData->LoSHearingRange;
			HearingConfig->SetMaxAge(WalkerData->HearingMaxAge);

			HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
			HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;  //팀 ID 설정후 삭제
			HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; //팀 ID 설정 후 삭제

			AIPerception->ConfigureSense(*SightConfig);
			AIPerception->ConfigureSense(*HearingConfig);
		}
	}
}



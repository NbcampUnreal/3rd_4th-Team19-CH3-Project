#include "Enemy/Task/BTTask_FindRandomLocation.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "GameData/EnemyStatDataStruct.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
    NodeName = TEXT("Find Random Location");

	UWorld* World = GetWorld();
	if (World)
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UDataTableManager* DataTableMgr = GameInstance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);
		FEnemyStatDataStruct* WalkerData = DataTableMgr->GetData<FEnemyStatDataStruct>(EDataType::EnemyStat, 1);

		if (WalkerData)
		{
			SearchRadius = WalkerData->SearchRadius;
		}
	}

    // 이 키는 Vector(위치) 타입만 받겠다고 필터 설정
    LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocation, LocationKey));
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* MyPawn = AIController->GetPawn();
    if (!MyPawn) return EBTNodeResult::Failed;

    //내비시스템 바뀔 가능성 있음.
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()); 
    if (!NavSystem) return EBTNodeResult::Failed;

    FNavLocation RandomLocation;
    bool bFound = NavSystem->GetRandomReachablePointInRadius(
        MyPawn->GetActorLocation(), 
        SearchRadius,                
        RandomLocation               
    );

    if (bFound)
    {
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsVector(LocationKey.SelectedKeyName, RandomLocation.Location);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}

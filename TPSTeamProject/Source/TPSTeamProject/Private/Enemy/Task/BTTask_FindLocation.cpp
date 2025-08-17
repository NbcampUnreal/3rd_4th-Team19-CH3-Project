#include "Enemy/Task/BTTask_FindLocation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_FindLocation::UBTTask_FindLocation()
{
    NodeName = TEXT("Find Location");

    LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindLocation, LocationKey));
}

EBTNodeResult::Type UBTTask_FindLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* MyPawn = AIController->GetPawn();
    if (!MyPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return EBTNodeResult::Failed;

    FNavLocation ProjectedLocation;
    bool bFound = NavSystem->ProjectPointToNavigation(
        PointLocation,
        ProjectedLocation,
        FVector(50.0f, 50.0f, 200.f)
    );

    if (bFound)
    {
        BlackboardComp->SetValueAsVector(LocationKey.SelectedKeyName, ProjectedLocation.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

#include "Enemy/Task/BTTask_FindBehindTargetLocation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_FindBehindTargetLocation::UBTTask_FindBehindTargetLocation()
{
    NodeName = TEXT("Find Behind Target Location");

    LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindBehindTargetLocation, LocationKey));
}

EBTNodeResult::Type UBTTask_FindBehindTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* MyPawn = AIController->GetPawn();
    if (!MyPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return EBTNodeResult::Failed;

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
    if (!TargetActor) return EBTNodeResult::Failed;

    FVector TargetLocation = TargetActor->GetActorLocation();

    FVector Foward2D = TargetActor->GetActorForwardVector();
    Foward2D.Z = 0.0f;
    Foward2D.Normalize();

    FVector BehindLocation = TargetLocation - Foward2D * DistanceBehindPlayer;

    FNavLocation ProjectedLocation;
    bool bFound = NavSystem->ProjectPointToNavigation(
        BehindLocation,
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


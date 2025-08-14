#include "Enemy/Task/BTTask_FindAvoidTargetLocation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindAvoidTargetLocation::UBTTask_FindAvoidTargetLocation()
{
    NodeName = TEXT("Find Avoid Target Location");
}

EBTNodeResult::Type UBTTask_FindAvoidTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return EBTNodeResult::Failed;

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
    if (!TargetActor) return EBTNodeResult::Failed;

    FVector TargetLocation = TargetActor->GetActorLocation();
    FVector RandomDir = FMath::VRand();

    PointLocation = TargetLocation + RandomDir * Distance;

    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    return Result;
}

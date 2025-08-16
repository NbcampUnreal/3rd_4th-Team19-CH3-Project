#include "Enemy/Task/BTTask_FindBehindTargetLocation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindBehindTargetLocation::UBTTask_FindBehindTargetLocation()
{
    NodeName = TEXT("Find Behind Target Location");
}

EBTNodeResult::Type UBTTask_FindBehindTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return EBTNodeResult::Failed;

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
    if (!TargetActor) return EBTNodeResult::Failed;

    FVector TargetLocation = TargetActor->GetActorLocation();

    FVector Foward2D = TargetActor->GetActorForwardVector();
    Foward2D.Z = 0.0f;
    Foward2D.Normalize();

    PointLocation = TargetLocation - Foward2D * DistanceBehindPlayer;

    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    return Result;
}


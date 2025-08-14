#include "Enemy/Task/BTDecorator_DistanceChecker.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_DistanceChecker::UBTDecorator_DistanceChecker()
{
    NodeName = TEXT("Distance Checker");
}

bool UBTDecorator_DistanceChecker::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    FDistanceMemory* Memory = (FDistanceMemory*)NodeMemory;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return false;

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
    APawn* MyPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
    if (!TargetActor || !MyPawn) return false;

    float Distance = FVector::Dist(MyPawn->GetActorLocation(), TargetActor->GetActorLocation());

    bool bInsideRange = Distance < EnterDistance;
    bool bResult = bInsideRange || (Memory->bWasInsideRange && Distance < ExitDistance);

    Memory->bWasInsideRange = bInsideRange;

    return bResult;
}

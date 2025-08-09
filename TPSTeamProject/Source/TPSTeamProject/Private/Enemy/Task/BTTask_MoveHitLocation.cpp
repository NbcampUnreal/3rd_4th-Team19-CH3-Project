#include "Enemy/Task/BTTask_MoveHitLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_MoveHitLocation::UBTTask_MoveHitLocation()
{
	NodeName = TEXT("Move To Hit Location");
}

void UBTTask_MoveHitLocation::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (TaskResult == EBTNodeResult::Succeeded || TaskResult == EBTNodeResult::Aborted)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(TEXT("WasDamaged"), false);
		}
	}
}

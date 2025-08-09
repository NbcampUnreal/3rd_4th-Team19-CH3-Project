#include "Enemy/Task/BTTask_MoveHeardLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_MoveHeardLocation::UBTTask_MoveHeardLocation()
{
	NodeName = TEXT("Move To Heard Location");
}

void UBTTask_MoveHeardLocation::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (TaskResult == EBTNodeResult::Succeeded || TaskResult == EBTNodeResult::Aborted)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(TEXT("HeardNoise"), false);
		}
	}
}


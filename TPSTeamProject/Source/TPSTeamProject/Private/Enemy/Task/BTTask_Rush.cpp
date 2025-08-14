#include "Enemy/Task/BTTask_Rush.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/EnemyCharacter.h"

UBTTask_Rush::UBTTask_Rush()
{
	NodeName = TEXT("Rush To Player");
}

EBTNodeResult::Type UBTTask_Rush::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AAIController* AIController = OwnerComp.GetAIOwner();

	APawn* AIPawn = AIController->GetPawn();

	AEnemyCharacter* AIChar = Cast<AEnemyCharacter>(AIPawn);

	if (BlackboardComp && AIChar)
	{
		BlackboardComp->SetValueAsBool(TEXT("TaskRunning"), true);
		AIChar->UpdateMovementSpeed();
	}

	return Result;
}

void UBTTask_Rush::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AAIController* AIController = OwnerComp.GetAIOwner();

	APawn* AIPawn = AIController->GetPawn();

	AEnemyCharacter* AIChar = Cast<AEnemyCharacter>(AIPawn);
	
	if (BlackboardComp && AIChar)
	{
		BlackboardComp->SetValueAsBool(TEXT("TaskRunning"), false);
		AIChar->UpdateMovementSpeed();
	}

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

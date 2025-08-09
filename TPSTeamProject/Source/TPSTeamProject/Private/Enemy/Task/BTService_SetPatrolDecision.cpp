#include "Enemy/Task/BTService_SetPatrolDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_SetPatrolDecision::UBTService_SetPatrolDecision()
{
	NodeName = TEXT("Set Patrol Decision");
	bNotifyBecomeRelevant = true;
	Interval = 1.0f;
}

void UBTService_SetPatrolDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	int32 RandomValue = FMath::RandRange(0, 1);
	BlackboardComp->SetValueAsInt(TEXT("PatrolDecision"), RandomValue);
}


#include "Enemy/Task/BTService_CalculateAttakRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_CalculateAttakRange::UBTService_CalculateAttakRange()
{
	NodeName = TEXT("Calculate Attack Range");
	Interval = 0.2f;
}

void UBTService_CalculateAttakRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!BlackboardComp || !AIController) return;

	APawn* MyPawn = AIController->GetPawn();
	if (!MyPawn) return;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	if (!TargetActor)
	{
		BlackboardComp->SetValueAsBool(TEXT("IsInAttackRange"), false);
		return;
	}

	const float Distance = FVector::Dist(MyPawn->GetActorLocation(), TargetActor->GetActorLocation());

	bool bInAttackRange = BlackboardComp->GetValueAsBool(TEXT("IsInAttackRange"));

	if (!bInAttackRange)
	{
		if (Distance <= EnterRange)
			bInAttackRange = true;
	}
	else
	{
		if (Distance > ExitRange)
			bInAttackRange = false;
	}

	BlackboardComp->SetValueAsBool(TEXT("IsInAttackRange"), bInAttackRange);
}


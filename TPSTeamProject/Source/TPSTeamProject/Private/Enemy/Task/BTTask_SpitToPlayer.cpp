#include "Enemy/Task/BTTask_SpitToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SpitToPlayer::UBTTask_SpitToPlayer()
{
	NodeName = TEXT("Spit To Player");
}

EBTNodeResult::Type UBTTask_SpitToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AActor* SelfActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("SelfActor")));
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	if (!SelfActor || !TargetActor) return EBTNodeResult::Failed;

	FVector ToTarget = (TargetActor->GetActorLocation() - SelfActor->GetActorLocation()).GetSafeNormal();
	FVector SelfForward = SelfActor->GetActorForwardVector();

	const float FaceThreshold = 0.9f;

	if (FVector::DotProduct(SelfForward, ToTarget) < FaceThreshold) return EBTNodeResult::Failed;

	return Result;
}

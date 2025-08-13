#include "Enemy/Task/BTTask_SpitToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/EnemyCharacter.h"

UBTTask_SpitToPlayer::UBTTask_SpitToPlayer()
{
	NodeName = TEXT("Spit To Player");
}

EBTNodeResult::Type UBTTask_SpitToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	AEnemyCharacter* AIChar = Cast<AEnemyCharacter>(AIPawn);
	if (!AIChar) return EBTNodeResult::Failed;

	AActor* SelfActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("SelfActor")));
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	if (!SelfActor || !TargetActor) return EBTNodeResult::Failed;

	FVector ToTarget = (TargetActor->GetActorLocation() - SelfActor->GetActorLocation()).GetSafeNormal();
	FVector SelfForward = SelfActor->GetActorForwardVector();

	const float FaceThreshold = 0.9f;

	if (FVector::DotProduct(SelfForward, ToTarget) < FaceThreshold) return EBTNodeResult::Failed;

	if (SpitMontage)
	{
		UAnimInstance* AnimInstance = AIChar->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FOnMontageEnded OnMontageEndedDelegate;
			OnMontageEndedDelegate.BindUObject(this, &UBTTask_SpitToPlayer::OnMontageEnded, &OwnerComp);

			AnimInstance->Montage_Play(SpitMontage);
			AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, SpitMontage);

			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_SpitToPlayer::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	if (!bInterrupted)
	{

		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTTask_SpitToPlayer::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	AEnemyCharacter* AIChar = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AIChar && AIChar->GetMesh() && AIChar->GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = AIChar->GetMesh()->GetAnimInstance();
		if (AnimInstance->Montage_IsPlaying(nullptr))
		{
			AnimInstance->Montage_Stop(0.25f);
		}
	}
}
#include "Enemy/Task/BTTask_AttackPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/EnemyCharacter.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
	NodeName = TEXT("Attack Player");
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	AEnemyCharacter* AIChar = Cast<AEnemyCharacter>(AIPawn);
	if (!AIChar) return EBTNodeResult::Failed;

	if (AttackMontages.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, AttackMontages.Num() - 1);
		UAnimMontage* SelectedMontage = AttackMontages[RandomIndex];

		if (SelectedMontage)
		{
			UAnimInstance* AnimInstance = AIChar->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				FOnMontageEnded OnMontageEndedDelegate;
				OnMontageEndedDelegate.BindUObject(this, &UBTTask_AttackPlayer::OnMontageEnded, &OwnerComp);

				AnimInstance->Montage_Play(SelectedMontage);
				AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, SelectedMontage);

				UE_LOG(LogTemp, Warning, TEXT("Playing attack montage at index %d"), RandomIndex);
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_AttackPlayer::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	if (!bInterrupted)
	{
		//비헤이비어 트리에 태스크 성공여부 알리기.
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTTask_AttackPlayer::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
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
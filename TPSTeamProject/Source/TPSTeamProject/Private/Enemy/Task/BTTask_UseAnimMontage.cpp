#include "Enemy/Task/BTTask_UseAnimMontage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/EnemyCharacter.h"

UBTTask_UseAnimMontage::UBTTask_UseAnimMontage()
{
	NodeName = TEXT("Use Anim Montage");
}

EBTNodeResult::Type UBTTask_UseAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	AEnemyCharacter* AIChar = Cast<AEnemyCharacter>(AIPawn);
	if (!AIChar) return EBTNodeResult::Failed;

	if (AnimMontages.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, AnimMontages.Num() - 1);
		UAnimMontage* SelectedMontage = AnimMontages[RandomIndex];

		if (SelectedMontage)
		{
			UAnimInstance* AnimInstance = AIChar->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				FOnMontageEnded OnMontageEndedDelegate;
				OnMontageEndedDelegate.BindUObject(this, &UBTTask_UseAnimMontage::OnMontageEnded, &OwnerComp);

				AnimInstance->Montage_Play(SelectedMontage);
				AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, SelectedMontage);

				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_UseAnimMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
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

void UBTTask_UseAnimMontage::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
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

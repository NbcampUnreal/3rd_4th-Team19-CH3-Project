#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpitToPlayer.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_SpitToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SpitToPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SpitMontage;
};

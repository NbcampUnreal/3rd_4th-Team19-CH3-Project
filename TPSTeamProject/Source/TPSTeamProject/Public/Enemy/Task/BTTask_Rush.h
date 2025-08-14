#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_Rush.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_Rush : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UBTTask_Rush();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};

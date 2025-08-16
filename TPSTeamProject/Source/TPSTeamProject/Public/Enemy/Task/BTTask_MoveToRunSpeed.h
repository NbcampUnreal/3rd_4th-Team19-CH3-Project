#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToRunSpeed.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_MoveToRunSpeed : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTTask_MoveToRunSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
};

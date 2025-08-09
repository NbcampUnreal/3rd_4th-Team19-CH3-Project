#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveHeardLocation.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_MoveHeardLocation : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveHeardLocation();

protected:
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};

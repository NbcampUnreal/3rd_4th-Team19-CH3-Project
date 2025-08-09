#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveHitLocation.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_MoveHitLocation : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveHitLocation();

protected:
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};

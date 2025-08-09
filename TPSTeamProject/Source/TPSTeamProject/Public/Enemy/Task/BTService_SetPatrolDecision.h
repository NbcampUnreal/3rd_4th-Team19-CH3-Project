#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetPatrolDecision.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTService_SetPatrolDecision : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SetPatrolDecision();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

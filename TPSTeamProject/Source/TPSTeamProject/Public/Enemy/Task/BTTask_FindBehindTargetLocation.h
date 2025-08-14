#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindBehindTargetLocation.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_FindBehindTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindBehindTargetLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector LocationKey;

	UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = "100.0"))
	float DistanceBehindPlayer = 100.0f;
};

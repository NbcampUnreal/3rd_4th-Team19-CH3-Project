#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindLocation.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_FindLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector LocationKey;

	UPROPERTY(EditAnywhere, Category = "Location")
	FVector PointLocation = FVector::ZeroVector;
	
};

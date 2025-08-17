#pragma once

#include "CoreMinimal.h"
#include "Enemy/Task/BTTask_FindLocation.h"
#include "BTTask_FindBehindTargetLocation.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_FindBehindTargetLocation : public UBTTask_FindLocation
{
	GENERATED_BODY()
	
public:
	UBTTask_FindBehindTargetLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = "100.0"))
	float DistanceBehindPlayer = 100.0f;
};

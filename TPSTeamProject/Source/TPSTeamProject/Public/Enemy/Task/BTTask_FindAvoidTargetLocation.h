#pragma once

#include "CoreMinimal.h"
#include "Enemy/Task/BTTask_FindLocation.h"
#include "BTTask_FindAvoidTargetLocation.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_FindAvoidTargetLocation : public UBTTask_FindLocation
{
	GENERATED_BODY()

public:
	UBTTask_FindAvoidTargetLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category="Distance")
	float Distance = 600.0f;
	
};

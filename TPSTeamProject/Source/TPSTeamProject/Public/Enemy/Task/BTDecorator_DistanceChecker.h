#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_DistanceChecker.generated.h"

USTRUCT()
struct FDistanceMemory
{
	GENERATED_BODY()

	bool bWasInsideRange = false;
};

UCLASS()
class TPSTEAMPROJECT_API UBTDecorator_DistanceChecker : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_DistanceChecker();

protected:
    UPROPERTY(EditAnywhere, Category = "Distance")
    float EnterDistance = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Distance")
    float ExitDistance = 600.0f;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector TargetActorKey;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FDistanceMemory); }
	
};

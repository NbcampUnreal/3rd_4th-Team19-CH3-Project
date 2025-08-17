#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CalculateAttakRange.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTService_CalculateAttakRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CalculateAttakRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "AttackRange")
	float EnterRange = 800.0f;
	UPROPERTY(EditAnywhere, Category = "AttackRange")
	float ExitRange = 850.0f;
};

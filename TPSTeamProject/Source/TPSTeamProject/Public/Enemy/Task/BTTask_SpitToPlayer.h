#pragma once

#include "CoreMinimal.h"
#include "Enemy/Task/BTTask_UseAnimMontage.h"
#include "BTTask_SpitToPlayer.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UBTTask_SpitToPlayer : public UBTTask_UseAnimMontage
{
	GENERATED_BODY()
	
public:
	UBTTask_SpitToPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyController.h"
#include "WalkerController.generated.h"

UCLASS()
class TPSTEAMPROJECT_API AWalkerController : public AEnemyController
{
	GENERATED_BODY()

public:
	AWalkerController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
};

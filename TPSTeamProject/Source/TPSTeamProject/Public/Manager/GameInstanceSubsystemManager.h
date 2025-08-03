#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameInstanceSubsystemManager.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UGameInstanceSubsystemManager : public UObject
{
	GENERATED_BODY()

public:
	UGameInstanceSubsystemManager();

	virtual void Init();

	
};

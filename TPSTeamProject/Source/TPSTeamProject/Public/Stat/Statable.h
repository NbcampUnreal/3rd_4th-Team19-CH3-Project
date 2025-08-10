#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Statable.generated.h"

UINTERFACE(MinimalAPI)
class UStatable : public UInterface
{
	GENERATED_BODY()
};

class TPSTEAMPROJECT_API IStatable
{
	GENERATED_BODY()

public:
	virtual void UpdateStats() = 0;

	
};

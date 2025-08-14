#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolObject.generated.h"

UINTERFACE(MinimalAPI)
class UPoolObject : public UInterface
{
	GENERATED_BODY()
};

class TPSTEAMPROJECT_API IPoolObject
{
	GENERATED_BODY()

public:
	virtual void OnFirstCreate() = 0;
	virtual void OnLastRelease() = 0;
	virtual void OnAlloc() = 0;
	virtual void OnFree() = 0;
};

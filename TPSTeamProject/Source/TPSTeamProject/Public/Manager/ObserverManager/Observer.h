#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Observer.generated.h"

enum class EMessageType : uint8;

UINTERFACE(MinimalAPI)
class UObserver : public UInterface
{
	GENERATED_BODY()
};

class TPSTEAMPROJECT_API IObserver
{
	GENERATED_BODY()

public:

	virtual void OnEvent(EMessageType InMsgType, int32 InParam) = 0;
};

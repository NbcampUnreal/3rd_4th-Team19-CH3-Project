#pragma once

#include "CoreMinimal.h"
#include "Manager/GameInstanceSubsystemManager.h"
#include "ObserverManager.generated.h"

class IObserver;
enum class EMessageType : uint8;

UCLASS()
class TPSTEAMPROJECT_API UObserverManager : public UGameInstanceSubsystemManager
{
	GENERATED_BODY()
	
public:
	UObserverManager();

	void Subscribe(IObserver* Observer);
	void UnSubscrive(IObserver* Observer);
	void SendEvent(EMessageType InMsgType, int32 InParam);

private:
	TArray<IObserver*> Observers;
};

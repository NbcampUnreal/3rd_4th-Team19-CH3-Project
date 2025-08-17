#include "Manager/GameInstanceSubsystem/ObserverManager.h"
#include "Manager/ObserverManager/Observer.h"

UObserverManager::UObserverManager()
{

}

void UObserverManager::Subscribe(IObserver* Observer)
{
	Observers.Add(Observer);
}

void UObserverManager::UnSubscrive(IObserver* Observer)
{
	Observers.Remove(Observer);
}

void UObserverManager::SendEvent(EMessageType InMsgType, int32 InParam)
{
	for (auto& Observer : Observers)
	{
		Observer->OnEvent(InMsgType, InParam);
	}
}

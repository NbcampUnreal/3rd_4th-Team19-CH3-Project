#include "TPSGameInstance.h"
#include "Manager\GameInstanceSubsystem\DataTableManager.h"
#include "Manager\GameInstanceSubsystem\ObserverManager.h"

void UTPSGameInstance::Init()
{
	Super::Init();

	Subsystems.Add(ESubsystemType::DataTable, NewObject<UDataTableManager>());
	Subsystems.Add(ESubsystemType::Observer, NewObject<UObserverManager>());

	for (auto Subsystem : Subsystems)
	{
		Subsystem.Value->Init();
	}
}


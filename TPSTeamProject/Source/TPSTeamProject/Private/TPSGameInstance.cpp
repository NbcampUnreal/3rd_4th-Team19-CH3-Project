#include "TPSGameInstance.h"
#include "Manager\GameInstanceSubsystem\DataTableManager.h"

void UTPSGameInstance::Init()
{
	Super::Init();

	Subsystems.Add(ESubsystemType::DataTable, NewObject<UDataTableManager>());

	for (auto Subsystem : Subsystems)
	{
		Subsystem.Value->Init();
	}
}


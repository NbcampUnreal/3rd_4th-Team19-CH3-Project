#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "Misc/Paths.h"

UDataTableManager::UDataTableManager()
{

}

void UDataTableManager::Init()
{
	Super::Init();

	FString DataTablePackagePath = TEXT("/Game/Data/DataTables");

	DataTables.Add(
		EDataType::Character,
		Cast<UDataTable>(
			StaticLoadObject(UDataTable::StaticClass(), 
			nullptr, 
			*FPaths::Combine(*DataTablePackagePath, *FString(TEXT("CharacterData.CharacterData")))
			)
		)
	);
}

UDataTable* UDataTableManager::GetTable(EDataType InType)
{
	UDataTable* DataTable = *DataTables.Find(InType);
	checkf(DataTable, TEXT("%s DataTable is not Exist"), *StaticEnum<EDataType>()->GetDisplayNameTextByValue((int64)InType).ToString());

	return DataTable;
}

#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "GameData/CharacterDataStruct.h"

UDataTableManager::UDataTableManager()
{

}

void UDataTableManager::Init()
{
	Super::Init();

	AddDataTable<FCharacterDataStruct>(EDataType::Character, FString(TEXT("CharacterData.CharacterData")));
}

UCacheDataTable* UDataTableManager::GetTable(EDataType InType)
{
	UCacheDataTable* DataTable = *DataTables.Find(InType);
	checkf(DataTable, TEXT("%s DataTable is not Exist"), *StaticEnum<EDataType>()->GetDisplayNameTextByValue((int64)InType).ToString());

	return DataTable;
}


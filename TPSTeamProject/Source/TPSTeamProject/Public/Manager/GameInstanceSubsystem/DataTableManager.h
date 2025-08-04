#pragma once

#include "CoreMinimal.h"
#include "Manager/GameInstanceSubsystemManager.h"
#include "GameData/CacheDataTable.h"
#include "DataTableManager.generated.h"

UENUM(BlueprintType)
enum class EDataType : uint8
{
	Character UMETA(DisplayName = "Character")
};

UCLASS()
class TPSTEAMPROJECT_API UDataTableManager : public UGameInstanceSubsystemManager
{
	GENERATED_BODY()

public:
	UDataTableManager();

	virtual void Init() override;

	UCacheDataTable* GetTable(EDataType);

	template<typename T>
	T* GetData(EDataType InType, int32 InKey)
	{
		UCacheDataTable* DataTable = GetTable(InType);

		return DataTable->FindRow<T>(InKey, FString(TEXT("Find Row")));
	}

private:
	UPROPERTY()
	TMap<EDataType, TObjectPtr<class UCacheDataTable>> DataTables;
	
	const FString TablePath = TEXT("/Game/Data/DataTables");

	template<typename T>
	void AddDataTable(EDataType InType, const FString& TableName)
	{
		UCacheDataTable* CacheTable = NewObject<UCacheDataTable>(this);
		UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *FPaths::Combine(*TablePath, *TableName)));

		CacheTable->Init<T>(DataTable);

		DataTables.Add(InType, CacheTable);
	}
	
};

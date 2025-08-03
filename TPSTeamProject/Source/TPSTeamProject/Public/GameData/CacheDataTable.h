#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TableKey.h"
#include "CacheDataTable.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UCacheDataTable : public UObject
{
	GENERATED_BODY()
	
public:
	UCacheDataTable() {}

	template<typename T>
	void Init(UDataTable* InDataTable)
	{
		static_assert(std::is_base_of<ITableKey, T>::value, "T must be a child of ITableKey");

		CacheDataTable = InDataTable;

		TArray<FName> RowNames = InDataTable->GetRowNames();
		for (auto RowName : RowNames)
		{
			T* Row = InDataTable->FindRow<T>(RowName, FString(TEXT("Find Row")));

			KeyList.Add(Row->GetKey(), RowName);
		}
	}

	FORCEINLINE UDataTable* GetTable() const
	{
		return CacheDataTable;
	}

	FORCEINLINE const FName& GetKeyName(int32 Key) const
	{
		return KeyList[Key];
	}
	
	template<typename T>
	T* FindRow(int32 Key, const FString& ContextString, bool bWarnIfRowMissing = true)
	{
		return CacheDataTable->FindRow<T>(GetKeyName(Key), ContextString, bWarnIfRowMissing);
	}

private:
	UPROPERTY()
	TMap<int32, FName> KeyList;
	UPROPERTY()
	TObjectPtr<UDataTable> CacheDataTable;


};

#pragma once

#include "CoreMinimal.h"
#include "Manager/GameInstanceSubsystemManager.h"
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

	UDataTable* GetTable(EDataType);

private:

	UPROPERTY()
	TMap<EDataType, TObjectPtr<class UDataTable>> DataTables;
	
};

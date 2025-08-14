#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatCalculater.generated.h"

enum class EGameStatType : uint8;
class UStatContainerCollection;
struct FStatContainer;

UCLASS()
class TPSTEAMPROJECT_API UStatCalculater : public UObject
{
	GENERATED_BODY()
	
public:
	UStatCalculater();

	void AddStats(FStatContainer* InStats);
	void RemoveStats(FStatContainer* InStats);
	void UpdateStat();

	int32 GetAtkDamage();
	int32 GetMaxHP();
	int32 GetDef();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StatCollection")
	TObjectPtr<UStatContainerCollection> StatCollection;
	UPROPERTY()
	TMap<EGameStatType, double> CurrentValueMap;



};

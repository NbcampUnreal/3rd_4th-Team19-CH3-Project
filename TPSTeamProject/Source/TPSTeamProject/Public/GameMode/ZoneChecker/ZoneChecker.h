#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZoneChecker.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UZoneChecker : public UObject
{
	GENERATED_BODY()
	
public:
	UZoneChecker();

	void Initialize(int32 InNormalCount, int32 InSpecialCount);
	bool IsClear();

	void DecreaseNormalMonsterCount();
	void DecreaseSpecialMonsterCount();

protected:


private:
	int32 NormalMonsterCount;
	int32 SpecialMonsterCount;

};

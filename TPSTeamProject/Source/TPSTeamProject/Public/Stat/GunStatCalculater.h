#pragma once

#include "CoreMinimal.h"
#include "Stat/StatCalculater.h"
#include "GunStatCalculater.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UGunStatCalculater : public UStatCalculater
{
	GENERATED_BODY()

public:
	UGunStatCalculater();

	int32 GetMaxAmmoCount();

private:
	UPROPERTY()
	int32 CurrentAmmo;
	
};

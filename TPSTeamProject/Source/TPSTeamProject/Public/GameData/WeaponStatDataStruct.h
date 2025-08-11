#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TableKey.h"
#include "WeaponStatDataStruct.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AssaultRifle,
	SubmachineGun,
	ShotGun,
	SniperRifle,
	Pistol
};

USTRUCT(BlueprintType)
struct TPSTEAMPROJECT_API FWeaponStatDataStruct : public FTableRowBase, public ITableKey
{
	GENERATED_BODY()

public:
	FWeaponStatDataStruct()
		: Damage(1)
		, RateOfFire(0.f)
		, MagazineSize(0.f)
		, ReloadSpeed(0.f)
		, Type(EWeaponType::AssaultRifle)
	{
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStat Data")
	double Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStat Data")
	double RateOfFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStat Data")
	double MagazineSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStat Data")
	double ReloadSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStat Data")
	EWeaponType Type;

	virtual int32 GetKey() const override
	{
		return static_cast<int32>(Type);
	}
};

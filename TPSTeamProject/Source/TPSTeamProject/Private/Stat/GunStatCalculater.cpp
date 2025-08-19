#include "Stat/GunStatCalculater.h"
#include "Stat/StatTypes.h"

UGunStatCalculater::UGunStatCalculater()
{

}

int32 UGunStatCalculater::GetMaxAmmoCount()
{
	auto MaxAmmoPtr = CurrentValueMap.Find(EGameStatType::MaxAmmoCount);
	return MaxAmmoPtr ? *MaxAmmoPtr : 0;
}

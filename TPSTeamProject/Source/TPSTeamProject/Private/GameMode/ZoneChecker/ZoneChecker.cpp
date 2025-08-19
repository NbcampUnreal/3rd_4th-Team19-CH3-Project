#include "GameMode/ZoneChecker/ZoneChecker.h"

UZoneChecker::UZoneChecker()
{
	
}

void UZoneChecker::Initialize(int32 InNormalCount, int32 InSpecialCount)
{
	NormalMonsterCount = InNormalCount;
	SpecialMonsterCount = InSpecialCount;
}

bool UZoneChecker::IsClear()
{
	return NormalMonsterCount == 0 && SpecialMonsterCount == 0;
}

void UZoneChecker::DecreaseNormalMonsterCount()
{
	--NormalMonsterCount;
}

void UZoneChecker::DecreaseSpecialMonsterCount()
{
	--SpecialMonsterCount;
}

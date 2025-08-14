#include "Stat/StatCalculater.h"
#include "Stat/StatContainerCollection.h"

UStatCalculater::UStatCalculater()
{
	StatCollection = CreateDefaultSubobject<UStatContainerCollection>(TEXT("Stat"));
}

void UStatCalculater::AddStats(FStatContainer* InStats)
{
	StatCollection->AddContainer(InStats);
}

void UStatCalculater::RemoveStats(FStatContainer* InStats)
{
	StatCollection->RemoveContainer(InStats);
}

void UStatCalculater::UpdateStat()
{
	static UEnum* EGameStatPtr = StaticEnum<EGameStatType>();
	int32 EnumNum = EGameStatPtr->NumEnums();
	for (int i = 0; i < EnumNum; i++)
	{
		EGameStatType StatType = static_cast<EGameStatType>(i);
		double StatValue = StatCollection->GetValue(StatType);
		if (FMath::IsNearlyZero(StatValue))
		{
			continue;
		}

		if (double* StatValuePtr = CurrentValueMap.Find(StatType))
		{
			*StatValuePtr = StatValue;
		}
		else
		{
			CurrentValueMap.Add(StatType, StatValue);
		}
	}
}

int32 UStatCalculater::GetAtkDamage()
{
	double* StatValuePtr = CurrentValueMap.Find(EGameStatType::Atk);
	return StatValuePtr ? *StatValuePtr : 0.0;
}

int32 UStatCalculater::GetMaxHP()
{
	double* StatValuePtr = CurrentValueMap.Find(EGameStatType::HP);
	return StatValuePtr ? *StatValuePtr : 0.0;
}

int32 UStatCalculater::GetDef()
{
	double* StatValuePtr = CurrentValueMap.Find(EGameStatType::Def);
	return StatValuePtr ? *StatValuePtr : 0.0;
}

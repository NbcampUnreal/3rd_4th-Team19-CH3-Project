#include "Stat/StatTypes.h"

TArray<EGameStatType> GetStatTypes()
{
	static UEnum* StatEnum = FindObject<UEnum>(nullptr, TEXT("/Script/TPSTeamProject.EGameStatType"));
	static TArray<EGameStatType> EnumArray;
	if (StatEnum && EnumArray.IsEmpty())
	{
		for (int32 i = 0; i < StatEnum->NumEnums(); i++)
		{
			EnumArray.Add(static_cast<EGameStatType>(StatEnum->GetValueByIndex(i)));
		}
	}

	return EnumArray;
}

#pragma once

#include "CoreMinimal.h"
#include "StatTypes.h"
#include "UUID.h"
#include "StatContainer.generated.h"

USTRUCT(BlueprintType)
struct TPSTEAMPROJECT_API FStatContainer
{
	GENERATED_BODY()

public:
	FStatContainer()
	{
		UUID = FUUID::CreateUUID();
	}

	FORCEINLINE void AddStat(EGameStatType InType, double InValue)
	{
		if (double* CurrentValuePtr = Stats.Find(InType))
		{
			*CurrentValuePtr += InValue;
		}
		else
		{
			Stats.Add(InType, InValue);
		}
	}

	FORCEINLINE void SubStat(EGameStatType InType, double InValue)
	{
		if (double* CurrentValuePtr = Stats.Find(InType))
		{
			if (*CurrentValuePtr < InValue)
			{
				*CurrentValuePtr = 0.0;

				return;
			}

			*CurrentValuePtr -= InValue;
		}
	}

	FORCEINLINE double GetStatValue(EGameStatType InType) const 
	{
		if (!Stats.Contains(InType))
		{
			return 0.0;
		}

		return Stats[InType];
	}

	virtual bool operator==(const FStatContainer& InContainer)
	{
		return UUID == InContainer.UUID;
	}

	FORCEINLINE FUUID GetUUID() const
	{
		return UUID;
	}

private:
	FUUID UUID;
	UPROPERTY()
	TMap<EGameStatType, double> Stats;
};

uint32 GetTypeHash(const FStatContainer& InContainer);

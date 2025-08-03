#pragma once

#include "CoreMinimal.h"

USTRUCT(BlueprintType)
struct TPSTEAMPROJECT_API FKeyStruct
{
	const FName& GetLevelKey(int32 InLevel)
	{
		return FName(FString::Printf(TEXT("LEVEL_%d"), InLevel));
	}

	const FName& GetEnumKey(UEnum EnumType)
	{

	}
};

#pragma once

#include "CoreMinimal.h"
#include "GameData/WeaponStatDataStruct.h"
#include "WeaponTypes.generated.h"

UENUM(BlueprintType)
enum class EModificationOp : uint8
{
	Add				UMETA(DisplayName = "Add"),
	Multiply		UMETA(DisplayName = "Multiply"),
	Override		UMETA(DisplayName = "Override")
};

USTRUCT(BlueprintType)
struct FStatModifier
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponStatDataStruct StatToModify;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EModificationOp Operation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value;
};
#pragma once

UENUM(BlueprintType)
enum class EGameStatType : uint8
{
	Atk,
	HP,
	Def,
	Stamina,
	MaxAmmoCount,
};

TArray<EGameStatType> GetStatTypes();

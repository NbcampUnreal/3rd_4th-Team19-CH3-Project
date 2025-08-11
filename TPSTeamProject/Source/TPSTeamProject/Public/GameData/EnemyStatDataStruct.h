#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TableKey.h"
#include "EnemyStatDataStruct.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Walker,
	Spitter,
	Brute,
	Screamer,
	Cloaker
};

USTRUCT(BlueprintType)
struct TPSTEAMPROJECT_API FEnemyStatDataStruct : public FTableRowBase, public ITableKey
{
	GENERATED_BODY()

public:
	FEnemyStatDataStruct()
		: Type(EEnemyType::Walker)
		, DefaultAtk(0.f)
		, DefaultHP(0.f)
		, WalkSpeed (0.f)
		, RunSpeed (0.f)
		, SearchRadius (0.f)
		, SightRadius (0.f)
		, LoseSightRadius (0.f)
		, PeripheralVisionAngleDegress (0.f)
		, SightMaxAge (0.f)
		, HearingRange (0.f)
		, LoSHearingRange (0.f)
		, HearingMaxAge (0.f)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	EEnemyType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float DefaultAtk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float DefaultHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float SearchRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float SightRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float LoseSightRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float PeripheralVisionAngleDegress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float SightMaxAge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float HearingRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float LoSHearingRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStat Data")
	float HearingMaxAge;

	virtual int32 GetKey() const override
	{
		return static_cast<int32>(Type);
	}

};

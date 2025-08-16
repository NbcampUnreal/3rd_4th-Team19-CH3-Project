#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TableKey.h"
#include "EnemySkillDataStruct.generated.h"

UENUM(BlueprintType)
enum class EEnemySkillType : uint8
{
	SpitterSkill
};

USTRUCT(BlueprintType)
struct TPSTEAMPROJECT_API FEnemySkillDataStruct : public FTableRowBase, public ITableKey
{
	GENERATED_BODY()

public:
	FEnemySkillDataStruct()
		: Type(EEnemySkillType::SpitterSkill)
		, ProjectileInitialSpeed(0.f)
		, ProjectileMaxSpeed(0.f)
		, ProjectileGravityScale(0.f)
		, PoolDamagePerTick(0.f)
		, PoolDamageInterval(0.f)
		, PoolLifeDuration(0.f)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySkill Data")
	EEnemySkillType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySkill Data")
	float ProjectileInitialSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySkill Data")
	float ProjectileMaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySkill Data")
	float ProjectileGravityScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySkill Data")
	float PoolDamagePerTick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySkill Data")
	float PoolDamageInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySkill Data")
	float PoolLifeDuration;


	virtual int32 GetKey() const override
	{
		return static_cast<int32>(Type);
	}

};

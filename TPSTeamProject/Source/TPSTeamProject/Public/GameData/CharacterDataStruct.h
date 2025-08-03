#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TableKey.h"
#include "CharacterDataStruct.generated.h"


USTRUCT(BlueprintType)
struct TPSTEAMPROJECT_API FCharacterDataStruct : public FTableRowBase, public ITableKey
{
	GENERATED_BODY()

public:
	FCharacterDataStruct()
		: Level(1)
		, DefaultAtk(0.f)
		, DefaultHP(0.f)
		, DefaultDef(0.f)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	float DefaultAtk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	float DefaultHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	float DefaultDef;

	virtual int GetKey() const override
	{
		return Level;
	}

};

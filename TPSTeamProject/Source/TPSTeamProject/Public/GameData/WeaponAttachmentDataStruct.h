#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TableKey.h"
#include "Weapon/WeaponTypes.h"
#include "Stat/StatTypes.h"
#include "WeaponAttachmentDataStruct.generated.h"

class AAttachmentActor;

USTRUCT(BlueprintType)
struct TPSTEAMPROJECT_API FWeaponAttachmentDataStruct : public FTableRowBase, public ITableKey
{
	GENERATED_BODY()

public:
	FWeaponAttachmentDataStruct()
		: SlotType(EAttachmentSlot::None)
		, Index(0)
		, MaxStatCount(0)
		, StatType1(EGameStatType::Atk)
		, StatValue1(0.0)
		, StatType2(EGameStatType::Atk)
		, StatValue2(0.0)
		, StatType3(EGameStatType::Atk)
		, StatValue3(0.0)
		, AttachmentMesh(nullptr)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	EAttachmentSlot SlotType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	int32 MaxStatCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	EGameStatType StatType1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	double StatValue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	EGameStatType StatType2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	double StatValue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	EGameStatType StatType3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	double StatValue3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	TSoftObjectPtr<USkeletalMesh> AttachmentMesh;

	virtual int32 GetKey() const override
	{
		return (Index << 15) + static_cast<int32>(SlotType);
	}

	static int32 GenerateKey(int32 Index, EAttachmentSlot InType);
};

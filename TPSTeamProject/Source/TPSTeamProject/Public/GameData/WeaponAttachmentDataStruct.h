#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TableKey.h"
#include "Weapon/WeaponTypes.h"
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
		, AttachmentMesh(nullptr)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	EAttachmentSlot SlotType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttachment Data")
	TSoftObjectPtr<USkeletalMesh> AttachmentMesh;

	virtual int32 GetKey() const override
	{
		return (Index << 16) + static_cast<int32>(SlotType);
	}
};

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponTypes.h"
#include "WeaponAttachmentDataAsset.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UWeaponAttachmentDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditDefaultsOnly, Category = "Stats")
	// TArray<FStatModifier> StatModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaticMesh")
	TSoftClassPtr<USkeletalMesh> AttachmentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	FName AttachmentSocketName;
};

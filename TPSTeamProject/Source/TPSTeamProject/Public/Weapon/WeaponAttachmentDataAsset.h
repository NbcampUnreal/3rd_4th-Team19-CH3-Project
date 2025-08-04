#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponAttachmentDataAsset.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UWeaponAttachmentDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// TODO: 나중에 부착물 관련 스탯 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaticMesh")
	TObjectPtr<USkeletalMesh> AttachmentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	FName AttachmentSocketName;
};

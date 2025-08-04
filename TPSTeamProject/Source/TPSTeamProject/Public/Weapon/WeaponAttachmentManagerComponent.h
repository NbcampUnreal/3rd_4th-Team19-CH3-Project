#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponAttachmentManagerComponent.generated.h"

class UWeaponAttachmentDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSTEAMPROJECT_API UWeaponAttachmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponAttachmentManagerComponent();

	// UI 등 외부에서 호출할 조립 기능 함수
	UFUNCTION(BlueprintCallable)
	void EquipAttachment(UWeaponAttachmentDataAsset* AttachmentData);

protected:
	// 현재 장착된 부착물 컴포넌트들을 관리하는 배열
	TArray<TObjectPtr<USkeletalMeshComponent>> AttachedComponents;
};

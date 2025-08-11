#include "Weapon/WeaponAttachmentManagerComponent.h"
#include "Weapon/WeaponAttachmentDataAsset.h"

UWeaponAttachmentManagerComponent::UWeaponAttachmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponAttachmentManagerComponent::EquipAttachment(UWeaponAttachmentDataAsset* AttachmentData)
{
	if (AttachmentData == nullptr || AttachmentData->AttachmentMesh == nullptr)
	{
		return;
	}

	// // 부착물 스태틱 메시 컴포넌트 동적 생성
	// if (USkeletalMeshComponent* AttachmentMeshComponent = NewObject<USkeletalMeshComponent>(
	// 	GetOwner(), USkeletalMeshComponent::StaticClass()))
	// {
	// 	AttachmentMeshComponent->RegisterComponent();
	// 	AttachmentMeshComponent->SetSkeletalMesh(AttachmentData->AttachmentMesh);
	// 	AttachmentMeshComponent->AttachToComponent(GetOwner()->GetComponentByClass<USkeletalMeshComponent>(),
	// 	                                           FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	// 	                                           AttachmentData->AttachmentSocketName);
	// 	AttachedComponents.Add(AttachmentMeshComponent);
	// }
}

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EquipmentParts.generated.h"

struct FStatContainer;
struct FWeaponAttachmentDataStruct;
class UCacheDataTable;
class USkeletalMeshComponent;
enum class EAttachmentSlot : uint8;

UCLASS()
class TPSTEAMPROJECT_API UEquipmentParts : public UObject
{
	GENERATED_BODY()

public:
	UEquipmentParts();

	UFUNCTION()
	void Initialize(USkeletalMeshComponent* MeshComp, int32 Index, EAttachmentSlot InType);
	UFUNCTION()
	void ChangeItem(int32 Index);

	FORCEINLINE EAttachmentSlot GetType() const;
	FORCEINLINE FStatContainer* GetContainer() const;

	void Release();

protected:
	virtual void LoadMesh();


private:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> PartsMesh;
	UPROPERTY()
	TObjectPtr<UCacheDataTable> AttachDataTable;
	UPROPERTY()
	EAttachmentSlot Type;

	FWeaponAttachmentDataStruct* AttachmentData;
	FStatContainer* StatContainer;
	
	void UpdateStat();
};

#include "Equipment/EquipmentParts.h"

#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "GameData/WeaponAttachmentDataStruct.h"
#include "GameData/CacheDataTable.h"
#include "Stat/StatContainer.h"
#include "ActorComponents/StatsComponent.h"

#include "Engine/AssetManager.h"

UEquipmentParts::UEquipmentParts()
{
	StatContainer = new FStatContainer();
}

void UEquipmentParts::Initialize(USkeletalMeshComponent* MeshComp, int32 Index, EAttachmentSlot InType)
{
	PartsMesh = MeshComp;
	Type = InType;

	if (UWorld* World = GetWorld())
	{
		UTPSGameInstance* Instance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UDataTableManager* DataTableMgr = Instance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);
		AttachDataTable = DataTableMgr->GetTable(EDataType::WeaponAttachment);
	}

	ChangeItem(Index);
}

void UEquipmentParts::ChangeItem(int32 Index)
{
	int32 Key = FWeaponAttachmentDataStruct::GenerateKey(Index, Type);
	AttachmentData = AttachDataTable->FindRow<FWeaponAttachmentDataStruct>(Key, FString(TEXT("Get Attachment Data")));

	checkf(AttachmentData, TEXT("InValid Index Or Type. Please Use Correct Index Or Type"));

	UpdateStat();

	LoadMesh();
}

EAttachmentSlot UEquipmentParts::GetType() const
{
	return Type;
}

FStatContainer* UEquipmentParts::GetContainer() const
{
	return StatContainer;
}

void UEquipmentParts::Release()
{
	delete StatContainer;
	StatContainer = nullptr;
}

void UEquipmentParts::LoadMesh()
{
	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
	StreamableManager.RequestAsyncLoad(
		AttachmentData->AttachmentMesh.ToSoftObjectPath(),
		[this, MeshToLoad = AttachmentData->AttachmentMesh]()
		{
			if (USkeletalMesh* LoadMesh = MeshToLoad.Get())
			{
				PartsMesh->SetSkeletalMesh(LoadMesh);
			}
		}
	);
}

void UEquipmentParts::UpdateStat()
{
	StatContainer->EmptyStatValue();

	StatContainer->AddStat(AttachmentData->StatType1, AttachmentData->StatValue1);
	StatContainer->AddStat(AttachmentData->StatType2, AttachmentData->StatValue2);
	StatContainer->AddStat(AttachmentData->StatType3, AttachmentData->StatValue3);
}

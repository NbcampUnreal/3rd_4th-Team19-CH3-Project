#include "Gun/GunActor.h"

#include "TPSGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "ActorComponents/StatsComponent.h"
#include "Stat/StatContainer.h"
#include "Stat/StatContainerCollection.h"
#include "Camera/CameraActor.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameData/WeaponAttachmentDataStruct.h"
#include "Kismet/GameplayStatics.h"
#include "Util/Component/ObjectTweenComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapon/WeaponTypes.h"

// Sets default values
AGunActor::AGunActor()
{
	PrimaryActorTick.bCanEverTick = true;

	/*RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(RootComp);*/

	GripBaseComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunBase_grip"));
	SetRootComponent(GripBaseComp);

	GripIKLockatorComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GripIKLockator"));
	GripIKLockatorComp->SetupAttachment(GripBaseComp);
	HullComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hull"));
	HullComp->SetupAttachment(GripBaseComp);

	ButtComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Butt"));
	ButtComp->SetupAttachment(HullComp);
	HandguardComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Handguard"));
	HandguardComp->SetupAttachment(HullComp);

	GripAttachmentComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GripAttachment"));
	GripAttachmentComp->SetupAttachment(HandguardComp);
	BarrelComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Barrel"));
	BarrelComp->SetupAttachment(HandguardComp);

	MuzzleComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Muzzle"));
	MuzzleComp->SetupAttachment(BarrelComp);

	/*ScopeComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Scope"));
	ScopeComp->SetupAttachment(BarrelComp);*/

	MuzzleLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MuzzleLight"));
	MuzzleLight->SetupAttachment(MuzzleComp);

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(MuzzleComp);

	ScopeCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ScopeCamera"));
	ScopeCameraComp->SetupAttachment(HullComp);
	//SightCameraComp = CreateDefaultSubobject<ACameraActor>(TEXT("SightCamera"));
	//SightCameraComp->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	//SightCameraComp->GetRootComponent()->AttachToComponent(HullComp, FAttachmentTransformRules::KeepRelativeTransform);
	ScopeComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Scope"));
	ScopeComp->SetupAttachment(HullComp);
	MountMagComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MountMag"));
	MountMagComp->SetupAttachment(HullComp);

	MagComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mag"));
	MagComp->SetupAttachment(MountMagComp);

	ShootFireTweenComp = CreateDefaultSubobject<UObjectTweenComponent>(TEXT("ShootFireTweenComp"));
	ShootFireParticleTweenComp = CreateDefaultSubobject<UObjectTweenComponent>(TEXT("ShootFireParticleTweenComp"));
}

void AGunActor::BeginPlay()
{
	Super::BeginPlay();

	// 관리용 맵(AttachmentCompMap) 초기화
	AttachmentCompMap.Add(EAttachmentSlot::Grip, GripBaseComp);
	AttachmentCompMap.Add(EAttachmentSlot::Hull, HullComp);
	AttachmentCompMap.Add(EAttachmentSlot::Butt, ButtComp);
	AttachmentCompMap.Add(EAttachmentSlot::Handguard, HandguardComp);
	AttachmentCompMap.Add(EAttachmentSlot::GripAttachment, GripAttachmentComp);
	AttachmentCompMap.Add(EAttachmentSlot::Barrel, BarrelComp);
	AttachmentCompMap.Add(EAttachmentSlot::Muzzle, MuzzleComp);
	AttachmentCompMap.Add(EAttachmentSlot::Scope, ScopeComp);
	AttachmentCompMap.Add(EAttachmentSlot::MountMag, MountMagComp);
	AttachmentCompMap.Add(EAttachmentSlot::Mag, MagComp);

	StatCollection = NewObject<UStatContainerCollection>();

	Params.AddIgnoredActor(this);

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		CameraManager = PlayerController->PlayerCameraManager;
	}

	if (IsValid(ShootFireTweenComp))
	{
		ShootFireTweenComp->AddBoolEvent(this, FName("OnFireLight"), 0.0f);
		ShootFireTweenComp->AddBoolEvent(this, FName("OffFireLight"), 0.1f);

		ShootFireParticleTweenComp->AddBoolEvent(this, FName("OnMuzzleParticle"), 0.0f);
		//ShootFireParticleTweenComp->AddBoolEvent(this, FName("OffMuzzleParticle"), 0.2f);
	}
}

// Called every frame
void AGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunActor::Fire()
{
	if (IsValid(CameraManager) == false)
	{
		return;
	}

	const FVector& FireStartPointLocation = CameraManager->GetCameraLocation();
	const FVector& FireEndPointLocation = FireStartPointLocation * FireRange;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		FireStartPointLocation,
		FireEndPointLocation,
		ECollisionChannel::ECC_Visibility,
		Params
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();

		UE_LOG(LogTemp, Log, TEXT("Actor Name : %s, Damage : %lf"), *HitActor->GetFName().ToString(), StatCollection->GetValue(EGameStatType::Atk));
	}

	ShootFireTweenComp->PlayFromStart();
	ShootFireParticleTweenComp->PlayFromStart();
}

void AGunActor::UpdateStats()
{
	StatCollection->EmptyContainer();
	for (auto StatComp : StatComponents)
	{
		StatCollection->AddContainer(StatComp->GetStatContainer());
	}
}

UCameraComponent* AGunActor::GetScopeCameraComp() const
{
	return ScopeCameraComp;
}

ACameraActor* AGunActor::GetSightCameraComp() const
{
	return SightCameraComp;
}

void AGunActor::SetAttachmentByIndex(EAttachmentSlot Slot, int32 Index)
{
	checkf(Slot!=EAttachmentSlot::None, TEXT("AGunActor::SetAttachmentByIndex : None Slot"));

	// TODO: 파츠 교체할때 스탯 변경되면 기존부착물 스탯 먼저 제거

	LoadAndSetAttachmentMesh(Slot, Index);

	// TODO: 새 부착물 스탯적용
}

void AGunActor::LoadAndSetAttachmentMesh(EAttachmentSlot Slot, int32 Index)
{
	UWorld* World = GetWorld();
	checkf(World, TEXT("AGunActor::LoadAndSetAttachmentMesh : No World"));
	UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
	checkf(GameInstance, TEXT("AGunActor::LoadAndSetAttachmentMesh : No GameInstance"));
	UDataTableManager* DataTableMgr = GameInstance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);
	checkf(DataTableMgr, TEXT("AGunActor::LoadAndSetAttachmentMesh : No DataTableMgr"));

	//UDataTable* WeaponAttachmentDataTable = DataTableMgr->GetTable(EDataType::WeaponAttachment)->GetTable();

	FWeaponAttachmentDataStruct* WeaponAttachmentDataRow = DataTableMgr->GetData<FWeaponAttachmentDataStruct>(
		EDataType::WeaponAttachment, (Index << 16) + static_cast<int32>(Slot));
	checkf(WeaponAttachmentDataRow, TEXT("AGunActor::LoadAndSetAttachmentMesh : No WeaponAttachmentData"));

	USkeletalMeshComponent* TargetComponent = AttachmentCompMap.FindRef(Slot);
	checkf(TargetComponent, TEXT("AGunActor::LoadAndSetAttachmentMesh : No TargetComponent"));

	//const FString SlotString = UEnum::GetValueAsString(Slot);
	//const FName RowName = FName(*FString::Printf(TEXT("%s%d"), *SlotString, Index));

	if (WeaponAttachmentDataRow->SlotType == Slot && WeaponAttachmentDataRow->Index == Index)
	{
		UE_LOG(LogTemp, Warning, TEXT("SlotType: %s, Index: %d"), *UEnum::GetValueAsString(Slot), Index);
		
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(WeaponAttachmentDataRow->AttachmentMesh.ToSoftObjectPath(),
			[TargetComponent, MeshToLoad = WeaponAttachmentDataRow->AttachmentMesh]()
			{
				if (USkeletalMesh* LoadedMesh = MeshToLoad.Get())
				{
					TargetComponent->SetSkeletalMesh(LoadedMesh);
				}
			});
	}
}

void AGunActor::OnFireLight()
{
	MuzzleLight->SetVisibility(true);
}

void AGunActor::OffFireLight()
{
	MuzzleLight->SetVisibility(false);
}

void AGunActor::OnMuzzleParticle()
{
	if (MuzzleParticle)
	{
		MuzzleParticleComp = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			MuzzleParticle,
			FirePoint->GetComponentLocation(),
			FirePoint->GetComponentRotation()
		);

		/*if (!MuzzleParticleComp)
		{
			MuzzleParticleComp = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				MuzzleParticle,
				FirePoint->GetRelativeLocation(),
				FirePoint->GetRelativeRotation(),
				false
			);
		}
		else
		{
			MuzzleParticleComp->SetVisibility(true);
		}*/
	}
}

void AGunActor::OffMuzzleParticle()
{
	if (MuzzleParticleComp && IsValid(MuzzleParticleComp))
	{
		MuzzleParticleComp->SetVisibility(false);
	}
}



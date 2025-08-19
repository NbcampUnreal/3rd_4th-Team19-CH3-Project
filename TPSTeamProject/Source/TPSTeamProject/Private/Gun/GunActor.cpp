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
#include "Equipment/EquipmentParts.h"
#include "Kismet/GameplayStatics.h"
#include "Util/Component/ObjectTweenComponent.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapon/WeaponTypes.h"
#include "Stat/GunStatCalculater.h"
#include "Character/ShooterCharacter.h"
#include "Equipment/EquipType.h"
#include "State/ShooterState.h"
#include "Manager/GameInstanceSubsystem/ObserverManager.h"
#include "Manager/ObserverManager/MessageType.h"
#include "Perception/AISense_Hearing.h"
#include "NiagaraFunctionLibrary.h"
#include "drawdebughelpers.h"

AGunActor::AGunActor()
{
	PrimaryActorTick.bCanEverTick = false;

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

	MuzzleLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MuzzleLight"));
	MuzzleLight->SetupAttachment(MuzzleComp);

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(MuzzleComp);

	ScopeCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ScopeCamera"));
	ScopeCameraComp->SetupAttachment(HullComp);
	ScopeComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Scope"));
	ScopeComp->SetupAttachment(HullComp);
	MountMagComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MountMag"));
	MountMagComp->SetupAttachment(HullComp);
	ShellEjectPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShellEjectPoint"));
	ShellEjectPoint->SetupAttachment(HullComp);

	MagComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mag"));
	MagComp->SetupAttachment(MountMagComp);

	ShootFireTweenComp = CreateDefaultSubobject<UObjectTweenComponent>(TEXT("ShootFireTweenComp"));
	ShootFireParticleTweenComp = CreateDefaultSubobject<UObjectTweenComponent>(TEXT("ShootFireParticleTweenComp"));

	StatCalculater = CreateDefaultSubobject<UGunStatCalculater>(TEXT("StatCalculater"));
}

void AGunActor::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		CameraManager = PlayerController->PlayerCameraManager;
		if (AShooterState* ShooterState = PlayerController->GetPlayerState<AShooterState>())
		{
			ShooterState->AddStatCalculater(StatCalculater);
		}

		AShooterCharacter* Character = Cast<AShooterCharacter>(PlayerController->GetCharacter());
		Character->AddStatCalculater(StatCalculater);
	}

	InitializeAttachment();

	Params.AddIgnoredActor(this);

	if (IsValid(ShootFireTweenComp))
	{
		ShootFireTweenComp->AddBoolEvent(this, FName("OnFireLight"), 0.0f);
		ShootFireTweenComp->AddBoolEvent(this, FName("OffFireLight"), 0.1f);

		ShootFireParticleTweenComp->AddBoolEvent(this, FName("OnMuzzleParticle"), 0.0f);
	}
}

bool AGunActor::Fire()
{
	if (IsValid(CameraManager) == false)
	{
		return false;
	}

	bool bCanFire = IsExistAmmo();
	if (bCanFire)
	{
		CurrentAmmoCount--;

		AActor* CurrentCameraActor = CameraManager->GetViewTarget();
		UCameraComponent* CurrentCameraComp = CurrentCameraActor->FindComponentByClass<UCameraComponent>();

		const FVector& FireStartPointLocation = CurrentCameraComp->GetComponentLocation();
		const FRotator& CurrentCameraRotater = CurrentCameraComp->GetComponentRotation();
		const FVector& FireEndPointLocation = FireStartPointLocation + CurrentCameraRotater.Quaternion().GetForwardVector() * FireRange;

		bool bHit = false;
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			int32 ViewportSizeX, ViewportSizeY;
			PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

			FVector2D ScreenCenter = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);

			FVector StartLocation;
			FVector Direction;
			PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, StartLocation, Direction);

			FVector EndLocation = StartLocation + Direction * 5000.f;

			bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartLocation,
				EndLocation,
				ECollisionChannel::ECC_Visibility,
				Params
			);

			//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, 10.f);
		}

		UNiagaraSystem* BulletNiagara = nullptr;
		float DecalLifeSpan = 0.f;
		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();

			UE_LOG(LogTemp, Log, TEXT("Actor Name : %s, Damage : %d"), *HitActor->GetName(), StatCalculater->GetAtkDamage());

			if (HitActor->IsA<APawn>())
			{
				float Damage = StatCalculater->GetAtkDamage();

				FName HitBoneName = HitResult.BoneName;
				UE_LOG(LogTemp, Warning, TEXT("Hit BoneName: %s"), *HitBoneName.ToString());
				if (HitBoneName == HitBone_Head)
				{
					Damage *= 1.5f;
				}
				else
				{
					Damage *= 1.0f;
				}

				UGameplayStatics::ApplyPointDamage(
					HitActor,
					Damage,
					HitResult.TraceEnd - HitResult.TraceStart,
					HitResult,
					GetOwner()->GetInstigatorController(),
					this,
					nullptr
				);

				BulletNiagara = EnemyHitEffect;
				DecalLifeSpan = 0.5f;
			}
			else
			{
				BulletNiagara = NonEnemyHitEffect;
				DecalLifeSpan = 5.f;
			}

			if (BulletNiagara)
			{
				/*FRotator DecalRotation = (-HitResult.ImpactNormal).Rotation();*/
				
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					this,
					BulletNiagara,
					HitResult.Location,
					FRotator::ZeroRotator,
					FVector(1.0f)
				);
			}
		}

		UGameplayStatics::SpawnDecalAtLocation(
			this,
			FireTrace,
			FVector(16.f, 16.f, 16.f),
			FirePoint->GetComponentLocation(),
			FRotator::ZeroRotator,
			0.5f
		);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			FireEffect,
			FirePoint->GetComponentLocation(),
			FRotator::ZeroRotator,
			FVector(1.0f)
		);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ShellEjectEffect,
			ShellEjectPoint->GetComponentLocation(),
			FRotator::ZeroRotator,
			FVector(1.0f)
		);

		//ShootFireTweenComp->PlayFromStart();
		//ShootFireParticleTweenComp->PlayFromStart();
	}
	/*else
	{

	}*/

	PlayFireSound(bCanFire);

	return bCanFire;
}

UCameraComponent* AGunActor::GetScopeCameraComp() const
{
	return ScopeCameraComp;
}

ACameraActor* AGunActor::GetSightCameraComp() const
{
	return SightCameraComp;
}

void AGunActor::ChangeParts(int32 Index, EAttachmentSlot InType)
{
	ensureMsgf(InType != EAttachmentSlot::None, TEXT("AGunActor::SetAttachmentByIndex : None Slot"));
	EquipPartsMap[InType]->ChangeItem(Index);

	StatCalculater->UpdateStat();

	UpdateMaxAmmoCount();
}

void AGunActor::Reload()
{
	CurrentAmmoCount = MaxAmmoCount;
}

UStatCalculater* AGunActor::GetStatCalculater() const
{
	return StatCalculater;
}

bool AGunActor::IsExistAmmo() const
{
	return CurrentAmmoCount > 0;
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
	}
}

void AGunActor::OffMuzzleParticle()
{
	if (MuzzleParticleComp && IsValid(MuzzleParticleComp))
	{
		MuzzleParticleComp->SetVisibility(false);
	}
}

void AGunActor::InitializeAttachment()
{
	DECLARE_DELEGATE_FourParams(FCombineMeshDelegate, USkeletalMeshComponent*, int32, EAttachmentSlot, const FName&);

	FCombineMeshDelegate CombineMeshDelegate;
	CombineMeshDelegate.BindLambda([this](USkeletalMeshComponent* InMeshComp, int32 Index, EAttachmentSlot InType, const FName& PartsName)
		{
			UEquipmentParts* EquipParts = NewObject<UEquipmentParts>(this, PartsName);

			EquipParts->Initialize(InMeshComp, Index, InType);
			StatCalculater->AddStats(EquipParts->GetContainer());
			EquipPartsMap.Add(InType, EquipParts);
		}
	);

	CombineMeshDelegate.Execute(GripBaseComp, 1, EAttachmentSlot::Grip, FName(TEXT("GripParts")));
	CombineMeshDelegate.Execute(HullComp, 1, EAttachmentSlot::Hull, FName(TEXT("HullParts")));
	CombineMeshDelegate.Execute(ButtComp, 1, EAttachmentSlot::Butt, FName(TEXT("ButtParts")));
	CombineMeshDelegate.Execute(HandguardComp, 1, EAttachmentSlot::Handguard, FName(TEXT("HandguardParts")));
	CombineMeshDelegate.Execute(GripAttachmentComp, 1, EAttachmentSlot::GripAttachment, FName(TEXT("GripAttachParts")));
	CombineMeshDelegate.Execute(BarrelComp, 1, EAttachmentSlot::Barrel, FName(TEXT("BarrelParts")));
	CombineMeshDelegate.Execute(MuzzleComp, 1, EAttachmentSlot::Muzzle, FName(TEXT("MuzzleParts")));
	//CombineMeshDelegate.Execute(ScopeComp, 1, EAttachmentSlot::Scope, FName(TEXT("ScopeParts")));
	CombineMeshDelegate.Execute(MountMagComp, 1, EAttachmentSlot::MountMag, FName(TEXT("MountMagParts")));
	CombineMeshDelegate.Execute(MagComp, 1, EAttachmentSlot::Mag, FName(TEXT("MagParts")));

	StatCalculater->UpdateStat();
	if (UWorld* World = GetWorld())
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UObserverManager* ObserverManager = GameInstance->GetSubsystem<UObserverManager>(ESubsystemType::Observer);

		ObserverManager->SendEvent(EMessageType::UpdateStat, -1);
	}

	CurrentAmmoCount =  StatCalculater->GetMaxAmmoCount();
	UpdateMaxAmmoCount();
}

void AGunActor::SetMeshToScope()
{
	if (!ScopeComp)
	{
		return;
	}

	USkeletalMeshComponent* ScopeSkeletalMeshComp = ScopeComp;
	ScopeSkeletalMeshComp->SetSkinnedAssetAndUpdate(ScopeSkeletalMeshComp->GetSkeletalMeshAsset(), true);
}

void AGunActor::TweakPlasticColorToScope()
{
	if (!ScopeComp)
	{
		return;
	}

	USkeletalMeshComponent* ScopeSkeletalMeshComp = ScopeComp;
	UMaterialInterface* ScopeMaterial = ScopeSkeletalMeshComp->GetMaterial(0);
	if (UMaterialInstanceDynamic* ScopeMaterialInstance = ScopeSkeletalMeshComp->CreateDynamicMaterialInstance(0, ScopeMaterial))
	{
		ScopeMaterialInstance->SetVectorParameterValue(FName(TEXT("PlasticTint")), FVector4(0.192708, 0.172001, 0.148152, 0));
	}
}

void AGunActor::SetVisibilityToScope()
{
	if (!ScopeComp)
	{
		return;
	}

	ScopeComp->SetVisibility(true);
}

void AGunActor::ApplyPaintTextureToScope()
{
	if (!ScopeComp)
	{
		return;
	}

	USkeletalMeshComponent* ScopeSkeletalMeshComp = ScopeComp;
	UMaterialInterface* ScopeMaterial = ScopeSkeletalMeshComp->GetMaterial(0);
	if (UMaterialInstanceDynamic* ScopeMaterialInstance = ScopeSkeletalMeshComp->CreateDynamicMaterialInstance(0, ScopeMaterial))
	{
		if (CamoTexture)
		{
			ScopeMaterialInstance->SetTextureParameterValue(FName(TEXT("Tile")), CamoTexture);
		}

		ScopeMaterialInstance->SetVectorParameterValue(FName(TEXT("Paint_Tint")), FVector4(0.5f, 0.5f, 0.5f, 0.f));
		ScopeMaterialInstance->SetScalarParameterValue(FName(TEXT("PaintClamp")), 1.f);
	}

}

void AGunActor::PlayFireSound(bool IsExistAmmo)
{
	USoundBase* PlaySoundBase = nullptr;
	if (IsExistAmmo == false)
	{
		if (EmptyAmmoSound)
		{
			PlaySoundBase = EmptyAmmoSound;
		}
	}
	else
	{
		if (FireBulletSound)
		{
			PlaySoundBase = FireBulletSound;
		}
	}

	if (PlaySoundBase)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PlaySoundBase,
			FirePoint->GetComponentLocation()
		);

		UAISense_Hearing::ReportNoiseEvent(
			GetWorld(),
			GetActorLocation(),
			50.0f,
			GetOwner(),
			0.0f,
			FName("Gunshot")
		);

		//DrawDebugSphere(
		//	GetWorld(),
		//	FirePoint->GetComponentLocation(),
		//	30.0f,
		//	12,
		//	FColor::Red,
		//	false,
		//	5.0f
		//);

		//DrawDebugSphere(
		//	GetWorld(),
		//	GetActorLocation(),
		//	30.0f,
		//	12,
		//	FColor::Green,
		//	false,
		//	5.0f
		//);
	}
}

void AGunActor::UpdateShooterStat()
{
	
}

void AGunActor::UpdateMaxAmmoCount()
{
	MaxAmmoCount = StatCalculater->GetMaxAmmoCount();
}


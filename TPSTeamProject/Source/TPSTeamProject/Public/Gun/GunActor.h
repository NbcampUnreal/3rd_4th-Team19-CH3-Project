#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stat/Statable.h"
#include "Stat/StatContainer.h"
#include "GunActor.generated.h"

struct FWeaponAttachmentDataStruct;
class USkeletalMeshComponent;
class UCameraComponent;
class UPointLightComponent;
class UObjectTweenComponent;
enum class EAttachmentSlot : uint8;

UCLASS()
class TPSTEAMPROJECT_API AGunActor : public AActor, public IStatable
{
	GENERATED_BODY()
	
public:	
	AGunActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Fire();

	virtual void UpdateStats() override;

	FORCEINLINE UCameraComponent* GetScopeCameraComp() const;

	FORCEINLINE ACameraActor* GetSightCameraComp() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attachment Customization")
	void SetAttachmentByIndex(EAttachmentSlot Slot, int32 Index);

protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
	TObjectPtr<USceneComponent> RootComp;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> GripBaseComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> GripIKLockatorComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> HullComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> ButtComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> HandguardComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> GripAttachmentComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> BarrelComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> MuzzleComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> ScopeComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> MountMagComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> MagComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<UCameraComponent> ScopeCameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<ACameraActor> SightCameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<UPointLightComponent> MuzzleLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USceneComponent> FirePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	int32 FireRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Tween")
	TObjectPtr<UObjectTweenComponent> ShootFireTweenComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Tween")
	TObjectPtr<UObjectTweenComponent> ShootFireParticleTweenComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UParticleSystem> MuzzleParticle;
	UPROPERTY()
	TObjectPtr<class UParticleSystemComponent> MuzzleParticleComp;

private:
	UPROPERTY()
	TSet<TObjectPtr<class UStatsComponent>> StatComponents;
	UPROPERTY()
	TObjectPtr<class UStatContainerCollection> StatCollection;

	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	TObjectPtr<APlayerCameraManager> CameraManager;

	TMap<EAttachmentSlot, TObjectPtr<USkeletalMeshComponent>> AttachmentCompMap;

	UPROPERTY()
	TMap<EAttachmentSlot, FStatContainer> EquippedAttachmentStats;

	UFUNCTION()
	void OnFireLight();
	UFUNCTION()
	void OffFireLight();
	UFUNCTION()
	void OnMuzzleParticle();
	UFUNCTION()
	void OffMuzzleParticle();

	void UnEquipAttachment(EAttachmentSlot Slot);

	void LoadAndSetAttachmentMesh(EAttachmentSlot Slot, int32 Index, FWeaponAttachmentDataStruct* WeaponAttachmentDataRow);

	FCollisionQueryParams Params;
};

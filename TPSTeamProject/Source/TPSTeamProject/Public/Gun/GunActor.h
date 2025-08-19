#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunActor.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;
class UPointLightComponent;
class UObjectTweenComponent;
class UEquipmentParts;
class UStatCalculater;
class UGunStatCalculater;
class UNiagaraSystem;
enum class EAttachmentSlot : uint8;

UCLASS()
class TPSTEAMPROJECT_API AGunActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGunActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterState")
	int32 CurrentAmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterState")
	int32 MaxAmmoCount;

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	bool Fire();

	FORCEINLINE UCameraComponent* GetScopeCameraComp() const;

	FORCEINLINE ACameraActor* GetSightCameraComp() const;
	
	UFUNCTION(BlueprintCallable)
	void ChangeParts(int32 Index, EAttachmentSlot InType);
	UFUNCTION(BlueprintCallable)
	void Reload();

	FORCEINLINE UStatCalculater* GetStatCalculater() const;

protected:
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USceneComponent> ShellEjectPoint;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScopeTexture")
	TObjectPtr<class UTexture> CamoTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	TObjectPtr<USoundBase> FireBulletSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	TObjectPtr<USoundBase> EmptyAmmoSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shoot")
	bool IsFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraSystem> EnemyHitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraSystem> NonEnemyHitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraSystem> FireEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ShellEjectEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UMaterialInterface> FireTrace;

	UFUNCTION(BlueprintCallable)
	void SetMeshToScope();
	UFUNCTION(BlueprintCallable)
	void TweakPlasticColorToScope();
	UFUNCTION(BlueprintCallable)
	void SetVisibilityToScope();
	UFUNCTION(BlueprintCallable)
	void ApplyPaintTextureToScope();

private:
	UPROPERTY()
	TObjectPtr<class UGunStatCalculater> StatCalculater;

	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	TObjectPtr<APlayerCameraManager> CameraManager;
	UPROPERTY()
	TMap<EAttachmentSlot, TObjectPtr<UEquipmentParts>> EquipPartsMap;

	UFUNCTION()
	void OnFireLight();
	UFUNCTION()
	void OffFireLight();
	UFUNCTION()
	void OnMuzzleParticle();
	UFUNCTION()
	void OffMuzzleParticle();
	UFUNCTION()
	void InitializeAttachment();
	UFUNCTION()
	void PlayFireSound(bool IsExistAmmo);
	UFUNCTION()
	void UpdateShooterStat();
	UFUNCTION()
	void UpdateMaxAmmoCount();

	UFUNCTION()
	FORCEINLINE bool IsExistAmmo() const;

	FCollisionQueryParams Params;
	const FName HitBone_Head = FName("head");
};

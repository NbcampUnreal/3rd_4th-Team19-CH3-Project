#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunActor.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;
class UPointLightComponent;
class UObjectTweenComponent;
class UEquipmentParts;
class UGunStatCalculater;
enum class EAttachmentSlot : uint8;

UCLASS()
class TPSTEAMPROJECT_API AGunActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGunActor();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void Fire();

	FORCEINLINE UCameraComponent* GetScopeCameraComp() const;

	FORCEINLINE ACameraActor* GetSightCameraComp() const;
	
	UFUNCTION(BlueprintCallable)
	void ChangeParts(int32 Index, EAttachmentSlot InType);

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
	void PlayFireSound();

	FCollisionQueryParams Params;
};

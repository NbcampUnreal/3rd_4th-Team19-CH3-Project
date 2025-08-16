#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy/Spitter/AcidPool.h"
#include "GameData/EnemySkillDataStruct.h"
#include "AcidProjecfile.generated.h"

UCLASS()
class TPSTEAMPROJECT_API AAcidProjecfile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAcidProjecfile();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	UPROPERTY(VisibleDefaultsOnly, Category="Projecfile")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category="Pool")
	TSubclassOf<class AAcidPool> AcidPoolClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataType")
	EEnemySkillType EnemySkillType;

	float DamageAmount = 0.0f;
};

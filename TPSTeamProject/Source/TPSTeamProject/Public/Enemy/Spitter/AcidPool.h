#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameData/EnemySkillDataStruct.h"
#include "AcidPool.generated.h"

UCLASS()
class TPSTEAMPROJECT_API AAcidPool : public AActor
{
	GENERATED_BODY()
	
public:	
	AAcidPool();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataType")
	EEnemySkillType EnemySkillType;

	UPROPERTY(EditAnywhere, Category="Properties")
	float DamagePerTick = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamageInterval = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float LifeDuration = 0.0f;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ApplyDamagePerTick();
	void DestroyPool();

private:
	FTimerHandle DamageTimerHandle;
	FTimerHandle LifeTimerHandle;

	TWeakObjectPtr<class AShooterCharacter> OverlappingPlayer;
};

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
#include "CloakerCharacter.generated.h"

UCLASS()
class TPSTEAMPROJECT_API ACloakerCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	ACloakerCharacter();
	
	UPROPERTY(EditAnywhere, Category="Materials")
	UMaterialInterface* TranslucentMaterial;
	UPROPERTY(EditAnywhere, Category = "Materials")
	TArray<UMaterialInterface*> OriginalMaterials;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void OnArmOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	void SetTranslucent(bool bTranslucent);

	bool bTakedDamage = false;

protected:
	virtual void BeginPlay();
};

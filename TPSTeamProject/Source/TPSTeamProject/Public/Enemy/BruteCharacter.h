#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
#include "BruteCharacter.generated.h"

UCLASS()
class TPSTEAMPROJECT_API ABruteCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	ABruteCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCapsuleComponent* RushCollision;

	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void EnableRushCollision(bool bEnable);

protected:
	virtual void BeginPlay() override;
};

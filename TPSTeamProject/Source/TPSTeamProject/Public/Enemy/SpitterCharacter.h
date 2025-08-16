#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
#include "Enemy/Spitter/AcidProjecfile.h"
#include "SpitterCharacter.generated.h"

UCLASS()
class TPSTEAMPROJECT_API ASpitterCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	ASpitterCharacter();

	UFUNCTION(BlueprintCallable)
	void SpitAcid();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spitter")
	TSubclassOf<class AAcidProjecfile> SpitProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spitter")
	FName SpitSocketName = "MouseSocket";
};

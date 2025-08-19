#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
#include "Enemy/WalkerCharacter.h"
#include "ScreamerCharacter.generated.h"

UCLASS()
class TPSTEAMPROJECT_API AScreamerCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	AScreamerCharacter();

	UFUNCTION(BlueprintCallable)
	void Scream();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnRadius = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 SpawnCount = 5;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<class AWalkerCharacter> EnemyToSpawnClass;
};

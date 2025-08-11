#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameData/EnemyStatDataStruct.h"
#include "EnemyController.generated.h"

class UAIPereptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class TPSTEAMPROJECT_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataType")
	EEnemyType EnemyType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const
	{
		return BlackboardComp;
	}

	void StartBehaviorTree();
};

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_EnableCollision.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Attack1,
	Attack2,
	BruteRush
};

UCLASS()
class TPSTEAMPROJECT_API UAnimNotifyState_EnableCollision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackType")
	EAttackType AttackType;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

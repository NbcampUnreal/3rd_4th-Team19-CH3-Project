#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpitToPlayer.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UAnimNotify_SpitToPlayer : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};

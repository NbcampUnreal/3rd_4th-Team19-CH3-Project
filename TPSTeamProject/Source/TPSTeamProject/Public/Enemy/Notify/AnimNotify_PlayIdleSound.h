#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayIdleSound.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UAnimNotify_PlayIdleSound : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* Sound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundConcurrency* Concurrency = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float SoundChance = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	FVector2D VolumeRange = FVector2D(0.8f, 1.2f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	FVector2D PitchRange = FVector2D(0.9f, 1.1f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* Attenuation = nullptr;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};

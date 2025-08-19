#include "Enemy/Notify/AnimNotify_PlayIdleSound.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_PlayIdleSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !Sound) return;

	UWorld* World = MeshComp->GetWorld();
	if (!World) return;

	if (FMath::FRand() > SoundChance) return;

	float Volume = FMath::FRandRange(VolumeRange.X, VolumeRange.Y);
	float Pitch = FMath::FRandRange(PitchRange.X, PitchRange.Y);

	FVector Location = MeshComp->GetComponentLocation();

	UGameplayStatics::PlaySoundAtLocation(
		World,
		Sound,
		Location,
		FRotator::ZeroRotator,
		Volume,
		Pitch,
		0.0f,
		Attenuation,
		Concurrency,
		MeshComp->GetOwner()
	);
}



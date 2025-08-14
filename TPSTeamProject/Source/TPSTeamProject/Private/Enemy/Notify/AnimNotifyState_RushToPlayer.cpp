#include "Enemy/Notify/AnimNotifyState_RushToPlayer.h"
#include "Enemy/BruteCharacter.h"

void UAnimNotifyState_RushToPlayer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UE_LOG(LogTemp, Warning, TEXT("Notify Begin"));

	ABruteCharacter* Character = Cast<ABruteCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->EnableRushCollision(true);
	}
}

void UAnimNotifyState_RushToPlayer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	UE_LOG(LogTemp, Warning, TEXT("Notify End"));

	ABruteCharacter* Character = Cast<ABruteCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->EnableRushCollision(false);
	}
}


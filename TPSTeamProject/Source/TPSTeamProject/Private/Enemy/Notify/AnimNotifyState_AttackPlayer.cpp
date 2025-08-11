#include "Enemy/Notify/AnimNotifyState_AttackPlayer.h"
#include "Enemy/EnemyCharacter.h"

void UAnimNotifyState_AttackPlayer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->bHasGivenDamage = false;
		Character->EnableRightArmCollision(true);
	}
}

void UAnimNotifyState_AttackPlayer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->EnableRightArmCollision(false);
	}
}

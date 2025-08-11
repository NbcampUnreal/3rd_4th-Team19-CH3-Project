#include "Enemy/Notify/AnimNotifyState_Attack2Player.h"
#include "Enemy/EnemyCharacter.h"

void UAnimNotifyState_Attack2Player::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->bHasGivenDamage = false;
		Character->EnableBothArmCollision(true);
	}
}

void UAnimNotifyState_Attack2Player::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->EnableBothArmCollision(false);
	}
}

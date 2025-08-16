#include "Enemy/Notify/AnimNotifyState_EnableCollision.h"
#include "Enemy/EnemyCharacter.h"
#include "Enemy/BruteCharacter.h"
#include "Enemy/CloakerCharacter.h"

void UAnimNotifyState_EnableCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (ABruteCharacter* BruteChar = Cast<ABruteCharacter>(MeshComp->GetOwner()))
	{
		if (AttackType == EAttackType::BruteRush)
		{
			BruteChar->EnableRushCollision(true);
		}
		else if (AttackType == EAttackType::Attack2)
		{
			BruteChar->EnableBothArmCollision(true);
		}
	}
	else if (AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(MeshComp->GetOwner()))
	{
		EnemyChar->bHasGivenDamage = false;

		if (AttackType == EAttackType::Attack1)
		{
			EnemyChar->EnableRightArmCollision(true);
		}
		else if (AttackType == EAttackType::Attack2)
		{
			EnemyChar->EnableBothArmCollision(true);
		}

		if (ACloakerCharacter* CloackerChar = Cast<ACloakerCharacter>(EnemyChar))
		{
			CloackerChar->SetTranslucent(false);
		}
	}
}


void UAnimNotifyState_EnableCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (ABruteCharacter* BruteChar = Cast<ABruteCharacter>(MeshComp->GetOwner()))
	{
		if (AttackType == EAttackType::BruteRush)
		{
			BruteChar->EnableRushCollision(false);
		}
		else if (AttackType == EAttackType::Attack2)
		{
			BruteChar->EnableBothArmCollision(false);
		}
	}
	else if (AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(MeshComp->GetOwner()))
	{
		if (AttackType == EAttackType::Attack1)
		{
			EnemyChar->EnableRightArmCollision(false);
		}
		else if (AttackType == EAttackType::Attack2)
		{
			EnemyChar->EnableBothArmCollision(false);
		}

		if (ACloakerCharacter* CloackerChar = Cast<ACloakerCharacter>(EnemyChar))
		{
			CloackerChar->SetTranslucent(true);
		}
	}
}
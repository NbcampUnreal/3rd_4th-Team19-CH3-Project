#include "Enemy/Notify/AnimNotify_SpitToPlayer.h"
#include "Enemy/SpitterCharacter.h"

void UAnimNotify_SpitToPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ASpitterCharacter* Character = Cast<ASpitterCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->SpitAcid();
	}
}

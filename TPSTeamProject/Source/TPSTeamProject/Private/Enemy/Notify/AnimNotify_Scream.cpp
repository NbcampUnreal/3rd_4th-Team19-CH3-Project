#include "Enemy/Notify/AnimNotify_Scream.h"
#include "Enemy/ScreamerCharacter.h"

void UAnimNotify_Scream::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AScreamerCharacter* Character = Cast<AScreamerCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->Scream();
	}
}

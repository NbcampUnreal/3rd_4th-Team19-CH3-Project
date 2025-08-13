#include "Enemy/Notify/AnimNotify_SpitToPlayer.h"
#include "Enemy/SpitterCharacter.h"

void UAnimNotify_SpitToPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ASpitterCharacter* Character = Cast<ASpitterCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Notify Called"));
		Character->SpitAcid();
		UE_LOG(LogTemp, Warning, TEXT("SpitAcid called and NEXT"));
	}
}

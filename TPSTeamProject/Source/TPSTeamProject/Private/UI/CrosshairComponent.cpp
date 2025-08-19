#include "UI/CrosshairComponent.h"
#include "Character/ShooterCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCrosshairComponent::UCrosshairComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    MinAimSize = 0.f;
    MaxAimSize = 500.f;
}


void UCrosshairComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}
}


void UCrosshairComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCrosshair(DeltaTime);
}


void UCrosshairComponent::VisibleCrosshair(bool bIsVisible)
{
    CrosshairWidget->VisibleCrosshair(bIsVisible);
}

void UCrosshairComponent::UpdateCrosshair(float DeltaTime)
{
    if (!CrosshairWidget)
    {
        return;
    }

    AActor* Owner = GetOwner();
    if (Owner)
    {
        FVector Velocity = Owner->GetVelocity();
        float Speed = Velocity.Size();

        AShooterCharacter* Character = Cast<AShooterCharacter>(Owner);
        bool IsJumping = Character->GetCharacterMovement()->IsFalling();
        bool IsShooting = Character->bIsShoot;

        if (Speed == 0 && AimSize == 0 && !IsShooting)
        {
            return;
        }

        if (IsJumping) 
        {
            AimSize += 2000.f * DeltaTime;
        }
        else 
        {
            if (Speed > 100)
            {
                AimSize += Speed * DeltaTime;
            }
            else if (Speed > 0)
            {
                AimSize -= FMath::Clamp(Speed * DeltaTime * 50, 0.0f, AimSize);
            }
            else
            {
                AimSize -= FMath::Clamp(2000.f * DeltaTime, 0.0f, AimSize);
            }
        }

        if (IsShooting)
        {
            AimSize += 2000.f * DeltaTime;
        }
        
        AimSize = FMath::Clamp(AimSize, MinAimSize, MaxAimSize);
        CrosshairWidget->UpdateCrosshairSize(AimSize);
    }
}


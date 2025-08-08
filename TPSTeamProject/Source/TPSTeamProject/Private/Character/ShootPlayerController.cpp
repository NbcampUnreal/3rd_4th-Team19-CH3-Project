#include "Character/ShootPlayerController.h"
#include "EnhancedInputSubsystems.h"

AShootPlayerController::AShootPlayerController()
	: InputMappingContext(nullptr)
	, ShootAction(nullptr)
	, CloseContactAction(nullptr)
	, ZoomAction(nullptr)
	, TurnAutoAction(nullptr)
{

}

void AShootPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			ensure(InputMappingContext);

			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

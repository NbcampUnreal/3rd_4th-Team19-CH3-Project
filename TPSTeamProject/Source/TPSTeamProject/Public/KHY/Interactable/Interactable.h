#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TPSTEAMPROJECT_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintable, Category = "Interaction")
	virtual void Interact(AActor* Interactor) = 0;
};

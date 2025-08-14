#pragma once

#include "KHY/Interactable/Interactable.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSTEAMPROJECT_API UItemComponent : public UActorComponent, public IInteractable
{
	GENERATED_BODY()

public:	
	UItemComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Interact(AActor* Interactor) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Amount;
};

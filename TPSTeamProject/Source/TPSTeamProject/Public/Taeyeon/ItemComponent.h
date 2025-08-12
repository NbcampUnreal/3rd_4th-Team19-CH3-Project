#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "Components/ActorComponent.h"
#include "GameData/ItemDataStruct.h"
#include "ItemComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSTEAMPROJECT_API UItemComponent : public UActorComponent, public IItemInterface
{
	GENERATED_BODY()

public:	
	UItemComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Interact(ACharacter* PlayerCharacter) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ItemQuantity;
};

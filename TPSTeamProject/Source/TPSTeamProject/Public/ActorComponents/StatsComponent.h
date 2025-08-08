#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stat/StatTypes.h"
#include "UUID.h"
#include "StatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSTEAMPROJECT_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsComponent();

protected:
	virtual void BeginPlay() override;


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	double GetStatValue(EGameStatType InType);

	FORCEINLINE struct FStatContainer* GetStatContainer() const;

	virtual bool operator==(const UStatsComponent& StatsComp)
	{
		return this->UUID == StatsComp.UUID;
	}

	FORCEINLINE FUUID GetUUID() const
	{
		return UUID;
	}

private:
	struct FStatContainer* StatContainer;
	FUUID UUID;
		
};

uint32 GetTypeHash(const UStatsComponent& StatsComp);

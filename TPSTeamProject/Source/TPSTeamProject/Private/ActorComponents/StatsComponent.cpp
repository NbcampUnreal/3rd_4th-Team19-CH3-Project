#include "ActorComponents/StatsComponent.h"
#include "Stat/StatContainer.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

double UStatsComponent::GetStatValue(EGameStatType InType)
{
	return StatContainer->GetStatValue(InType);
}

FStatContainer* UStatsComponent::GetStatContainer() const
{
	return StatContainer;
}

uint32 GetTypeHash(const UStatsComponent& StatsComp)
{
	return StatsComp.GetUUID().GetRealUUID();
}

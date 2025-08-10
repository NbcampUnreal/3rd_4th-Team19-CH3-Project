#include "Stat/StatContainerCollection.h"
#include "Stat/StatContainer.h"

UStatContainerCollection::UStatContainerCollection()
{

}

void UStatContainerCollection::AddContainer(FStatContainer* InContainer)
{
    StatContainers.Add(InContainer);
}

void UStatContainerCollection::RemoveContainer(const FStatContainer* InContainer)
{
    StatContainers.Remove(InContainer);
}

double UStatContainerCollection::GetValue(EGameStatType InType)
{
    double ReturnValue = 0.0;

    for (auto StatContainer : StatContainers)
    {
        ReturnValue += StatContainer->GetStatValue(InType);
    }

    return ReturnValue;
}

void UStatContainerCollection::EmptyContainer()
{
    StatContainers.Empty();
}

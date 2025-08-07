#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatTypes.h"
#include "StatContainerCollection.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UStatContainerCollection : public UObject
{
	GENERATED_BODY()

public:
	UStatContainerCollection();

	void AddContainer(struct FStatContainer* InContainer);
	double GetValue(EGameStatType InType);
	void RemoveContainer(const struct FStatContainer* InContainer);

private:
	UPROPERTY()
	TSet<struct FStatContainer*> StatContainers;
	
};

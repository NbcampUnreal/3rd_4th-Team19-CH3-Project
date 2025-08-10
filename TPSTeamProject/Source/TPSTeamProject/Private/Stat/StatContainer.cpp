#include "Stat/StatContainer.h"

uint32 GetTypeHash(const FStatContainer& InContainer)
{
	return InContainer.GetUUID().GetRealUUID();
}

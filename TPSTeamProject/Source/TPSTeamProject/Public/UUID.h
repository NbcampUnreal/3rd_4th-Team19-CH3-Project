#pragma once

#include "CoreMinimal.h"

struct TPSTEAMPROJECT_API FUUID
{
public:
	FUUID() : realUUID(0)
	{
	}

	static FUUID CreateUUID()
	{
		FUUID UUID = FUUID();
		UUID.realUUID = ++FUUID::uuid;

		return UUID;
	}

	uint32 GetUUID() const
	{
		return realUUID;
	}

	virtual bool operator==(const FUUID& InUUID)
	{
		return realUUID == InUUID.realUUID;
	}

private:
	int32 realUUID;
	static uint32 uuid;
};

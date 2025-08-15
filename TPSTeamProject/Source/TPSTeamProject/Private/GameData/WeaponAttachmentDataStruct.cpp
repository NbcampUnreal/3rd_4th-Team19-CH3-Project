#include "GameData/WeaponAttachmentDataStruct.h"

int32 FWeaponAttachmentDataStruct::GenerateKey(int32 Index, EAttachmentSlot InType)
{
	return (Index << 15) + static_cast<int32>(InType);
}

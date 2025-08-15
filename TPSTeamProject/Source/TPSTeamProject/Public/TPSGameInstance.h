#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

UENUM(BlueprintType)
enum class ESubsystemType : uint8
{
	DataTable,
	Observer,
};

UCLASS()
class TPSTEAMPROJECT_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	template<typename T>
	T* GetSubsystem(ESubsystemType);

private:

	UPROPERTY()
	TMap<ESubsystemType, TObjectPtr<class UGameInstanceSubsystemManager>> Subsystems;
	
};

template<typename T>
T* UTPSGameInstance::GetSubsystem(ESubsystemType InType)
{
	if (class UGameInstanceSubsystemManager* Subsystem = *Subsystems.Find(InType))
	{
		T* ReturnCast = Cast<T>(Subsystem);
		checkf(ReturnCast, TEXT("The template parameter is not UGameInstanceSubsystemManager."));

		return ReturnCast;
	}

	UE_LOG(LogTemp, Error, TEXT("This Subsystem not exist subsystems container"));

	return nullptr;
}

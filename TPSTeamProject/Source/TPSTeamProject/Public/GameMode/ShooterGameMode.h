#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Manager/ObserverManager/Observer.h"
#include "ShooterGameMode.generated.h"

class AShooterCharacter;
class AGunActor;
class UObjectTweenComponent;
class ABaseZone;

UCLASS()
class TPSTEAMPROJECT_API AShooterGameMode : public AGameMode, public IObserver
{
	GENERATED_BODY()

public:
	AShooterGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneList")
	TArray<TObjectPtr<ABaseZone>> ZoneList;

	UFUNCTION(BlueprintCallable)
	void ClearGame();

	virtual void BeginPlay() override;

	virtual void OnEvent(EMessageType InMsgType, int32 InParam) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TObjectPtr<AShooterCharacter> Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<AGunActor> GunActor;

private:
	int32 ClearZoneNum;

};

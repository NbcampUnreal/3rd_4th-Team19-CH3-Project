#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Manager/ObserverManager/Observer.h"
#include "ShooterGameMode.generated.h"

class AShooterCharacter;
class AGunActor;
class UObjectTweenComponent;
class ABaseZone;
class UGameOverWidget;

UCLASS()
class TPSTEAMPROJECT_API AShooterGameMode : public AGameMode, public IObserver
{
	GENERATED_BODY()

public:
	AShooterGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneList")
	TArray<TObjectPtr<ABaseZone>> ZoneList;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UGameOverWidget> GameOverWidgetClass;
	UPROPERTY()
	UGameOverWidget* GameOverWidgetInstance;

	UFUNCTION(BlueprintCallable)
	void ClearGame();

	virtual void BeginPlay() override;

	virtual void OnEvent(EMessageType InMsgType, int32 InParam) override;

	void ShowGameOverScreen();
	void HideGameOverScreen();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TObjectPtr<AShooterCharacter> Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<AGunActor> GunActor;

private:
	int32 ClearZoneNum;

};

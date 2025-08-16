#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Manager/ObserverManager/Observer.h"
#include "ShooterState.generated.h"

class UStatCalculater;
enum class EGameStatType : uint8;
enum class EMessageType : uint8;

UCLASS()
class TPSTEAMPROJECT_API AShooterState : public APlayerState, public IObserver
{
	GENERATED_BODY()

public:
	AShooterState();

	UFUNCTION(BlueprintCallable)
	void AddStatCalculater(UStatCalculater* StatCalculater);

	/*UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentAtk() const;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentHP() const;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentDef() const;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsDead() const;*/

	UFUNCTION(BlueprintCallable)
	void AddHP(int32 InValue);

	UFUNCTION(BlueprintCallable)
	void SubHP(int32 InValue);

	void UpdateStatValue();

	virtual void OnEvent(EMessageType InMsgType, int32 InParam);

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UStatCalculater>> StatCalculaters;
	UPROPERTY()
	TMap<EGameStatType, int32> StatValueMap;

	UPROPERTY()
	int32 CurrentHP;
};

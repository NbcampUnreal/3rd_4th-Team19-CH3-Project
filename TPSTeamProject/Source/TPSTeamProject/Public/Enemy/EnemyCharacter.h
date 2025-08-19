#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"  //팀 ID 설정
#include "Components/CapsuleComponent.h"
#include "GameData/EnemyStatDataStruct.h"
#include "EnemyCharacter.generated.h"

class AItem;
class UEnemyHPBarWidget;

UCLASS()
class TPSTEAMPROJECT_API AEnemyCharacter : public ACharacter ,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
	TSubclassOf<AItem> DropCoin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
	TObjectPtr<UDataTable> AttachmentLootTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AttachmentDropChance = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
	TSubclassOf<AItem> CommonAttachmentItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCapsuleComponent* BodyCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCapsuleComponent* RightArmCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCapsuleComponent* LeftArmCollision;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEnemyHPBarWidget> BossHPbarWidgetClass;
	UPROPERTY()
	UEnemyHPBarWidget* BossHPbarWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEnemyHPBarWidget> WalkerHPbarWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* WalkerHPbarWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataType")
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHealth = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Health = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float WalkSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RunSpeed = 0.0f;

	UPROPERTY()
	bool bHasGivenDamage = false;

	float BaseDamage;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; } //팀 ID
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override { TeamId = NewTeamID; } //팀 ID

	UFUNCTION()
	virtual void OnArmOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void ResetDamage();
	void MultiplyDamage(float Multiplier);

	void UpdateMovementSpeed();

	void EnableRightArmCollision(bool bEnable);
	void EnableBothArmCollision(bool bEnable);

	void OnDeath();
	void OnDeathAnimationFinished();

	void DropItems();
	void DropAttachment();

	void ShowBossHPbar();
	void UpdateBossHPbar();
	void HideBossHPbar();

	void ShowWalkerHPbar();
	void UpdateWalkerHPbar();
	void HideWalkerHPbar();

private:
	FGenericTeamId TeamId = FGenericTeamId(1); //팀 ID
	FTimerHandle DeathTimerHandle;

};

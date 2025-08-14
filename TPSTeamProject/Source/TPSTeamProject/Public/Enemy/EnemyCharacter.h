#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "GenericTeamAgentInterface.h"  //팀 ID 설정
#include "Components/CapsuleComponent.h"
#include "GameData/EnemyStatDataStruct.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class TPSTEAMPROJECT_API AEnemyCharacter : public ACharacter //,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCapsuleComponent* BodyCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCapsuleComponent* RightArmCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCapsuleComponent* LeftArmCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataType")
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathMontage;

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

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	//virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; } //팀 ID
	//virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override { TeamId = NewTeamID; } //팀 ID

	UFUNCTION()
	void OnArmOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void UpdateMovementSpeed();

	void EnableRightArmCollision(bool bEnable);
	void EnableBothArmCollision(bool bEnable);

	void OnDeath();
	void OnDeathAnimationFinished();

private:
	//FGenericTeamId TeamId = FGenericTeamId(1); //팀 ID
	FTimerHandle DeathTimerHandle;

};

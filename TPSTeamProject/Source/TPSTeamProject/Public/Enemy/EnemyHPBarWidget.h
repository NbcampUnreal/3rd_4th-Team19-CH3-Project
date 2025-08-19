#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Enemy/EnemyCharacter.h"
#include "EnemyHPBarWidget.generated.h"

UCLASS()
class TPSTEAMPROJECT_API UEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPbar;

	UPROPERTY()
	AEnemyCharacter* Target;

public:
	void InitEnemy(AEnemyCharacter* Enemy)
	{
		Target = Enemy;
		UpdateHP(Target->Health, Target->MaxHealth);
	}

	void UpdateHP(float CurrentHP, float MaxHP)
	{
		if (HPbar && MaxHP > 0)
		{
			HPbar->SetPercent(CurrentHP / MaxHP);
		}
	}
};

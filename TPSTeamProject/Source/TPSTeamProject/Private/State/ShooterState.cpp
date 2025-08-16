#include "State/ShooterState.h"
#include "Stat/StatCalculater.h"
#include "Stat/StatTypes.h"
#include "Manager/ObserverManager/MessageType.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/ObserverManager.h"

AShooterState::AShooterState()
{
	StatValueMap.Add(EGameStatType::Atk, 0);
	StatValueMap.Add(EGameStatType::HP, 0);
	StatValueMap.Add(EGameStatType::Def, 0);
}

void AShooterState::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UObserverManager* ObserverManager = GameInstance->GetSubsystem<UObserverManager>(ESubsystemType::Observer);

		ObserverManager->Subscribe(this);
	}
}

void AShooterState::AddStatCalculater(UStatCalculater* StatCalculater)
{
	StatCalculaters.Add(StatCalculater);
}

void AShooterState::UpdateStatValue()
{
	for (auto& StatValueMapElement : StatValueMap)
	{
		EGameStatType GameStatType = StatValueMapElement.Key;
		int32 StatValue = StatValueMapElement.Value;
		int32 TotalValue = 0;

		switch (GameStatType)
		{
			case EGameStatType::Atk:
			{
				for (int i = 0; i < StatCalculaters.Num(); i++)
				{
					TotalValue += StatCalculaters[i]->GetAtkDamage();
				}

				StatValueMap[EGameStatType::Atk] = TotalValue;

				UE_LOG(LogTemp, Log, TEXT("Atk : %d"), StatValueMap[EGameStatType::Atk]);
			}
			break;

			case EGameStatType::HP:
			{
				for (int i = 0; i < StatCalculaters.Num(); i++)
				{
					TotalValue += StatCalculaters[i]->GetMaxHP();
				}

				StatValueMap[EGameStatType::HP] = TotalValue;

				int32 Gap = TotalValue - StatValue;
				if (Gap > 0)
				{
					AddHP(Gap);
				}
				else
				{
					SubHP(Gap);
				}

				UE_LOG(LogTemp, Log, TEXT("Atk : %d"), StatValueMap[EGameStatType::Atk]);
			}
			break;

			case EGameStatType::Def:
			{
				for (int i = 0; i < StatCalculaters.Num(); i++)
				{
					TotalValue += StatCalculaters[i]->GetDef();
				}

				StatValueMap[EGameStatType::Def] = TotalValue;

				UE_LOG(LogTemp, Log, TEXT("Atk : %d"), StatValueMap[EGameStatType::HP]);
			}
			break;
		}
	}
}

void AShooterState::OnEvent(EMessageType InMsgType, int32 InParam)
{
	switch (InMsgType)
	{
		case EMessageType::UpdateStat:
		{
			UpdateStatValue();
		}
	}
}

void AShooterState::AddHP(int32 InValue)
{
	CurrentHP += InValue;
	if (StatValueMap[EGameStatType::HP] < CurrentHP)
	{
		CurrentHP = StatValueMap[EGameStatType::HP];
	}

}

void AShooterState::SubHP(int32 InValue)
{
	if (InValue < 0)
	{
		InValue = -InValue;
	}

	CurrentHP -= InValue;
	if (CurrentHP < 0)
	{
		CurrentHP = 0;
	}
}


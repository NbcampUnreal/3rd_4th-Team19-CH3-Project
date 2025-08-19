#include "GameMode/ShooterGameMode.h"
#include "KHY/Zone/BaseZone.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/ObserverManager.h"
#include "Manager/ObserverManager/MessageType.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GameOverWidget.h"

AShooterGameMode::AShooterGameMode()
{
	ClearZoneNum = 0;
}

void AShooterGameMode::ClearGame()
{
	ShowGameOverScreen();
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	DECLARE_DELEGATE_ThreeParams(InitializeZone, int32, int32, int32);

	TArray<AActor*> OutputActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseZone::StaticClass(), OutputActors);

	TArray<FString> NameArray = { TEXT("1WaveZone_C_1"), TEXT("2WaveZone_C_1") , TEXT("3WaveZone_C_1") , TEXT("4WaveZone_C_1") };

	for (const auto& ActorName : NameArray)
	{
		for (auto& OutputActor : OutputActors)
		{
			if (OutputActor && OutputActor->GetName().Equals(ActorName))
			{
				ZoneList.Add(Cast<ABaseZone>(OutputActor));

				break;
			}
		}
	}

	InitializeZone InitializeZoneDelegate;
	InitializeZoneDelegate.BindLambda([this](int32 Index, int32 InNormalCount, int32 InSpecialCount)
	{
		ZoneList[Index]->Initialize(InNormalCount, InSpecialCount);
	});

	InitializeZoneDelegate.Execute(0, 10, 1);
	InitializeZoneDelegate.Execute(1, 15, 1);
	InitializeZoneDelegate.Execute(2, 20, 1);
	InitializeZoneDelegate.Execute(3, 25, 1);

	if (UWorld* World = GetWorld())
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UObserverManager* ObserverManager = GameInstance->GetSubsystem<UObserverManager>(ESubsystemType::Observer);

		ObserverManager->Subscribe(this);
	}
}

void AShooterGameMode::OnEvent(EMessageType InMsgType, int32 InParam)
{
	if (!(InMsgType == EMessageType::KillNormal) && !(InMsgType == EMessageType::KillSpecial))
	{
		return;
	}

	int32 StageIndex = InParam - 1;
	if (ZoneList.IsValidIndex(StageIndex) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Param is unvalid"));

		return;
	}

	ABaseZone* CheckZone = ZoneList[StageIndex];
	if (InMsgType == EMessageType::KillNormal)
	{
		CheckZone->DecreaseNormal();
	}
	else if (InMsgType == EMessageType::KillSpecial)
	{
		CheckZone->DecreaseSpecial();
	}

	if (CheckZone->IsClear())
	{
		ClearZoneNum = InParam;

		CheckZone->DoorOpen();
		int32 NextStageIndex = ++StageIndex;
		if (ZoneList.IsValidIndex(NextStageIndex))
		{
			ZoneList[NextStageIndex]->DoorOpen();
		}
		else
		{
			ClearGame();
		}
	}
}


void AShooterGameMode::ShowGameOverScreen()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return;

	if (GameOverWidgetInstance)
	{
		HideGameOverScreen();
	}

	GameOverWidgetInstance = CreateWidget<UGameOverWidget>(PlayerController, GameOverWidgetClass);

	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void AShooterGameMode::HideGameOverScreen()
{
	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->RemoveFromParent();
		GameOverWidgetInstance = nullptr;
	}
}


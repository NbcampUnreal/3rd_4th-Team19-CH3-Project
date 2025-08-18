#include "Enemy/EnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyCharacter.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"

AEnemyController::AEnemyController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->SetMaxAge(5.0f);
	AIPerception->ConfigureSense(*SightConfig);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
	HearingConfig->SetMaxAge(5.0);
	AIPerception->ConfigureSense(*HearingConfig);

	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	SetGenericTeamId((FGenericTeamId(1)));

	if (AIPerception)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIPerception exists"));
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(
			this,
			&AEnemyController::OnPerceptionUpdated
		);
	}

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), false);
		BlackboardComp->SetValueAsBool(TEXT("WasDamaged"), false);
		BlackboardComp->SetValueAsBool(TEXT("HeardNoise"), false);
		if (!BlackboardComp->GetValueAsBool(TEXT("IsDead")))
		{
			StartBehaviorTree();
		}
	}
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UWorld* World = GetWorld();
	if (World)
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UDataTableManager* DataTableMgr = GameInstance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);
		FEnemyStatDataStruct* WalkerData = DataTableMgr->GetData<FEnemyStatDataStruct>(EDataType::EnemyStat, static_cast<int32>(EnemyType));

		if (WalkerData)
		{
			SightConfig->SightRadius = WalkerData->SightRadius;
			SightConfig->LoseSightRadius = WalkerData->LoseSightRadius;
			SightConfig->PeripheralVisionAngleDegrees = WalkerData->PeripheralVisionAngleDegress;
			SightConfig->SetMaxAge(WalkerData->SightMaxAge);

			HearingConfig->HearingRange = WalkerData->HearingRange;
			HearingConfig->LoSHearingRange = WalkerData->LoSHearingRange;
			HearingConfig->SetMaxAge(WalkerData->HearingMaxAge);
			 
			UE_LOG(LogTemp, Warning, TEXT("HearingRange: %f"), HearingConfig->HearingRange);

			AIPerception->ConfigureSense(*SightConfig);
			AIPerception->ConfigureSense(*HearingConfig);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("WalkerData is NULL!"));
		}
	}
}

void AEnemyController::StartBehaviorTree()
{
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Behavior Tree Asset Not Set"));
	}
}

void AEnemyController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	FString SenseName = TEXT("Unknown");

	if (Stimulus.Type == UAISense_Sight::StaticClass()->GetDefaultObject<UAISense>()->GetSenseID())
	{
		SenseName = TEXT("Sight");
	}
	else if (Stimulus.Type == UAISense_Hearing::StaticClass()->GetDefaultObject<UAISense>()->GetSenseID())
	{
		SenseName = TEXT("Hearing");
	}

	UE_LOG(LogTemp, Warning, TEXT("[Perception] Actor: %s, Sense: %s, Success: %d"),
		*GetNameSafe(Actor),
		*SenseName,
		Stimulus.WasSuccessfullySensed());

	if (!BlackboardComp) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
	{
		if (Actor == PlayerPawn)
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
				BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), true);
				UE_LOG(LogTemp, Warning, TEXT("Sight Succeed"));
			}
			else
			{
				BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), false);
			}
		}
	}

	if (Stimulus.Type == UAISense::GetSenseID(UAISense_Hearing::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hearing stimulus received!"));

		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComp->SetValueAsVector(TEXT("HeardLocation"), Stimulus.StimulusLocation);
			BlackboardComp->SetValueAsBool(TEXT("HeardNoise"), true);

			bool bHeardNoise = BlackboardComp->GetValueAsBool(TEXT("HeardNoise"));
			UE_LOG(LogTemp, Warning, TEXT("BB HeardNoise: %s"), bHeardNoise ? TEXT("true") : TEXT("false"));
			UE_LOG(LogTemp, Warning, TEXT("HeardNoise Succeed"));
		}
	}

	if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
	{
		AICharacter->UpdateMovementSpeed();
	}
}

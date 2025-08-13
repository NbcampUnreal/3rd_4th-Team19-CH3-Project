#include "Enemy/Spitter/AcidPool.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Character/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"

AAcidPool::AAcidPool()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(SceneComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(SceneComponent);
	SphereComponent->InitSphereRadius(150.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AAcidPool::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAcidPool::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAcidPool::OnOverlapEnd);

	UWorld* World = GetWorld();
	if (World)
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UDataTableManager* DataTableMgr = GameInstance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);
		FEnemySkillDataStruct* SpitterSkillData = DataTableMgr->GetData<FEnemySkillDataStruct>(EDataType::EnemySkill, static_cast<int32>(EnemySkillType));

		if (SpitterSkillData)
		{
			DamagePerTick = SpitterSkillData->PoolDamagePerTick;
			DamageInterval = SpitterSkillData->PoolDamageInterval;
			LifeDuration = SpitterSkillData->PoolLifeDuration;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SpitterSkillData is NULL!"));
		}
	}

	GetWorldTimerManager().SetTimer(
		DamageTimerHandle,
		this,
		&AAcidPool::ApplyDamagePerTick,
		DamageInterval,
		true
	);

	if (LifeDuration > 0.0f)
	{
		GetWorldTimerManager().SetTimer(
			LifeTimerHandle,
			this,
			&AAcidPool::DestroyPool,
			LifeDuration,
			false
		);
	}
}

void AAcidPool::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor))
	{
		OverlappingPlayer = Player;
	}
}

void AAcidPool::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappingPlayer.Get() == OtherActor)
	{
		OverlappingPlayer = nullptr;
	}
}

void AAcidPool::ApplyDamagePerTick()
{
	if (OverlappingPlayer.IsValid())
	{
		UGameplayStatics::ApplyDamage(
			OverlappingPlayer.Get(),
			DamagePerTick,
			nullptr,
			this,
			nullptr
		);
	}
}

void AAcidPool::DestroyPool()
{
	Destroy();
}

void AAcidPool::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	GetWorldTimerManager().ClearTimer(LifeTimerHandle);

	Super::EndPlay(EndPlayReason);
}


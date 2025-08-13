#include "Enemy/Spitter/AcidProjecfile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Character/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"

AAcidProjecfile::AAcidProjecfile()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(5.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));
	RootComponent = SphereComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);

	SphereComponent->OnComponentHit.AddDynamic(this, &AAcidProjecfile::OnHit);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void AAcidProjecfile::PostInitializeComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("AAcidProjecfile PostInitializeComponents Called"));

	Super::PostInitializeComponents();

	UWorld* World = GetWorld();
	if (World)
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UDataTableManager* DataTableMgr = GameInstance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);
		FEnemySkillDataStruct* SpitterSkillData = DataTableMgr->GetData<FEnemySkillDataStruct>(EDataType::EnemySkill, static_cast<int32>(EnemySkillType));

		if (SpitterSkillData)
		{
			ProjectileMovementComponent->InitialSpeed = SpitterSkillData->ProjectileInitialSpeed;
			ProjectileMovementComponent->MaxSpeed = SpitterSkillData->ProjectileMaxSpeed;
			ProjectileMovementComponent->ProjectileGravityScale = SpitterSkillData->ProjectileGravityScale;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SpitterSkillData is NULL!"));
		}
	}

	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;

	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileMovementComponent->InitialSpeed);
	ProjectileMovementComponent->Activate();
}

void AAcidProjecfile::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AcidProjecfile spawned"));

	if (GetInstigator())
	{
		SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	}
}

void AAcidProjecfile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile OnHit"));
	if (!AcidPoolClass)
	{
		Destroy();
		return;
	}

	FVector SpawnLoc = FVector::ZeroVector;
	FRotator SpawnRot = FRotator::ZeroRotator;

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn Hit!"));

		SpawnLoc = HitPawn->GetActorLocation() - 
			FVector(0.0f, 0.0f, HitPawn->GetSimpleCollisionHalfHeight());

		if (AShooterCharacter* Player = Cast<AShooterCharacter>(HitPawn))
		{

			UGameplayStatics::ApplyDamage(
				Player,
				DamageAmount,
				GetInstigatorController(),
				this,
				nullptr
			);
		}
	}
	else
	{
		SpawnLoc = Hit.ImpactPoint;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AAcidPool>(AcidPoolClass, SpawnLoc, SpawnRot, SpawnParams);

	Destroy();
}

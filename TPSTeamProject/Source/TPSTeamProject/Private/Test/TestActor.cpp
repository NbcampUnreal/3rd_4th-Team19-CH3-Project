#include "Test/TestActor.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "GameData/CharacterDataStruct.h"
#include "Stat/StatContainer.h"
#include "Stat/StatContainerCollection.h"

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (World)
	{
		UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(World->GetGameInstance());
		UDataTableManager* DataTableMgr = GameInstance->GetSubsystem<UDataTableManager>(ESubsystemType::DataTable);

		FCharacterDataStruct* CharacterData1 = DataTableMgr->GetData<FCharacterDataStruct>(EDataType::Character, 1);
		FCharacterDataStruct* CharacterData2 = DataTableMgr->GetData<FCharacterDataStruct>(EDataType::Character, 2);
		FCharacterDataStruct* CharacterData3 = DataTableMgr->GetData<FCharacterDataStruct>(EDataType::Character, 3);

		FStatContainer StatContainer1 = FStatContainer();
		FStatContainer StatContainer2 = FStatContainer();
		FStatContainer StatContainer3 = FStatContainer();

		StatContainer1.AddStat(EGameStatType::Atk, 100);
		StatContainer1.AddStat(EGameStatType::HP, 1000);

		StatContainer2.AddStat(EGameStatType::HP, 10000);
		StatContainer2.AddStat(EGameStatType::Def, 10);

		StatContainer3.AddStat(EGameStatType::Atk, 100000);

		UStatContainerCollection* StatContainerCollection = NewObject<UStatContainerCollection>();

		StatContainerCollection->AddContainer(&StatContainer1);
		StatContainerCollection->AddContainer(&StatContainer2);
		StatContainerCollection->AddContainer(&StatContainer3);

		UE_LOG(LogTemp, Log, TEXT("Atk : %lf, HP : %lf, Def : %lf"), StatContainerCollection->GetValue(EGameStatType::Atk), StatContainerCollection->GetValue(EGameStatType::HP), StatContainerCollection->GetValue(EGameStatType::Def));

		StatContainer1.SubStat(EGameStatType::Atk, 20);
		StatContainer2.AddStat(EGameStatType::Atk, 50);
		StatContainer3.AddStat(EGameStatType::HP, 70);
		StatContainer3.SubStat(EGameStatType::HP, 20);

		UE_LOG(LogTemp, Log, TEXT("Atk : %lf, HP : %lf, Def : %lf"), StatContainerCollection->GetValue(EGameStatType::Atk), StatContainerCollection->GetValue(EGameStatType::HP), StatContainerCollection->GetValue(EGameStatType::Def));

		StatContainerCollection->RemoveContainer(&StatContainer2);

		UE_LOG(LogTemp, Log, TEXT("UUID : %d"), StatContainer1.GetUUID().GetRealUUID());

		UE_LOG(LogTemp, Log, TEXT("Atk : %lf, HP : %lf, Def : %lf"), StatContainerCollection->GetValue(EGameStatType::Atk), StatContainerCollection->GetValue(EGameStatType::HP), StatContainerCollection->GetValue(EGameStatType::Def));

		// UE_LOG(LogTemp, Log, TEXT("Level : %d, DefaultAtk : %f, DefaultHP : %f, DefaultDef : %f"), CharacterData1->Level, CharacterData1->DefaultAtk, CharacterData1->DefaultHP, CharacterData1->DefaultDef);
		// UE_LOG(LogTemp, Log, TEXT("Level : %d, DefaultAtk : %f, DefaultHP : %f, DefaultDef : %f"), CharacterData2->Level, CharacterData2->DefaultAtk, CharacterData2->DefaultHP, CharacterData2->DefaultDef);
		// UE_LOG(LogTemp, Log, TEXT("Level : %d, DefaultAtk : %f, DefaultHP : %f, DefaultDef : %f"), CharacterData3->Level, CharacterData3->DefaultAtk, CharacterData3->DefaultHP, CharacterData3->DefaultDef);
	}
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


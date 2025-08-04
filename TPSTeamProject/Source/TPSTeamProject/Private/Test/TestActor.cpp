#include "Test/TestActor.h"
#include "TPSGameInstance.h"
#include "Manager/GameInstanceSubsystem/DataTableManager.h"
#include "GameData/CharacterDataStruct.h"

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
		FCharacterDataStruct * CharacterData2 = DataTableMgr->GetData<FCharacterDataStruct>(EDataType::Character, 2);
		FCharacterDataStruct* CharacterData3 = DataTableMgr->GetData<FCharacterDataStruct>(EDataType::Character, 3);
		UE_LOG(LogTemp, Log, TEXT("Level : %d, DefaultAtk : %f, DefaultHP : %f, DefaultDef : %f"), CharacterData1->Level, CharacterData1->DefaultAtk, CharacterData1->DefaultHP, CharacterData1->DefaultDef);
		UE_LOG(LogTemp, Log, TEXT("Level : %d, DefaultAtk : %f, DefaultHP : %f, DefaultDef : %f"), CharacterData2->Level, CharacterData2->DefaultAtk, CharacterData2->DefaultHP, CharacterData2->DefaultDef);
		UE_LOG(LogTemp, Log, TEXT("Level : %d, DefaultAtk : %f, DefaultHP : %f, DefaultDef : %f"), CharacterData3->Level, CharacterData3->DefaultAtk, CharacterData3->DefaultHP, CharacterData3->DefaultDef);
	}
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


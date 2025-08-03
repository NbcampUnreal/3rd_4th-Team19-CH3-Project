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
		UDataTable* CharacterDataTable = DataTableMgr->GetTable(EDataType::Character);

		ensure(CharacterDataTable);

		FCharacterDataStruct* CharacterData1 = CharacterDataTable->FindRow<FCharacterDataStruct>(FName(TEXT("Level_1")), FString(TEXT("Not Find Data")));
		FCharacterDataStruct* CharacterData2 = CharacterDataTable->FindRow<FCharacterDataStruct>(FName(TEXT("Level_2")), FString(TEXT("Not Find Data")));
		FCharacterDataStruct* CharacterData3 = CharacterDataTable->FindRow<FCharacterDataStruct>(FName(TEXT("Level_3")), FString(TEXT("Not Find Data")));
		UE_LOG(LogTemp, Log, TEXT("Level : %d, DefaultAtk : %f, DefaultHP : %f, DefaultDef : %f"), CharacterData1->Level, CharacterData1->DefaultAtk, CharacterData1->DefaultHP, CharacterData1->DefaultDef);
		UE_LOG(LogTemp, Log, TEXT("Level : %d, DefaultAtk : %f, DefaultHP : %f, DefaultDef : %f"), CharacterData2->Level, CharacterData2->DefaultAtk, CharacterData2->DefaultHP, CharacterData2->DefaultDef);
		UE_LOG(LogTemp, Log, TEXT("Level : %d, DefaultAtk : %f, DefaultHP : %f, DefaultDef : %f"), CharacterData3->Level, CharacterData3->DefaultAtk, CharacterData3->DefaultHP, CharacterData3->DefaultDef);
	}
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


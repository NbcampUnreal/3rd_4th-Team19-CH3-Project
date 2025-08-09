#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
//#include "GenericTeamAgentInterface.h"  //팀 ID 설정
#include "Components/CapsuleComponent.h"
#include "WalkerCharacter.generated.h"

UCLASS()
class TPSTEAMPROJECT_API AWalkerCharacter : public AEnemyCharacter //,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AWalkerCharacter();

protected:
	virtual void BeginPlay() override;

};

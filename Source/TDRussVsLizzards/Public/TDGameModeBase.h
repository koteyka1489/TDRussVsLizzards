// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDGameModeBase.generated.h"

class ABaseCreepPawn;
class ABaseCreepActor;



UCLASS()
class TDRUSSVSLIZZARDS_API ATDGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATDGameModeBase();
    virtual void StartPlay() override;

};

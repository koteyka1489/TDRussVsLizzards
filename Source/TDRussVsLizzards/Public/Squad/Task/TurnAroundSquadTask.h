// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Squad/Task/SquadBaseTask.h"
#include "TurnAroundSquadTask.generated.h"

/**
 *
 */
UCLASS()
class TDRUSSVSLIZZARDS_API UTurnAroundSquadTask : public USquadBaseTask
{
    GENERATED_BODY()

public:
    virtual void ExecuteTask() override;
};

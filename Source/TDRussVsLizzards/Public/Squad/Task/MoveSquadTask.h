// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Squad/Task/SquadBaseTask.h"
#include "MoveSquadTask.generated.h"

/**
 *
 */
UCLASS()
class TDRUSSVSLIZZARDS_API UMoveSquadTask : public USquadBaseTask
{
    GENERATED_BODY()

public:
    virtual void ExecuteTask() override;
};

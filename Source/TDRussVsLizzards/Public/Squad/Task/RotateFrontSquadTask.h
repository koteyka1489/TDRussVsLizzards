// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Squad/Task/SquadBaseTask.h"
#include "RotateFrontSquadTask.generated.h"

/**
 *
 */
UCLASS()
class TDRUSSVSLIZZARDS_API URotateFrontSquadTask : public USquadBaseTask
{
    GENERATED_BODY()

public:
    virtual void ExecuteTask() override;
};

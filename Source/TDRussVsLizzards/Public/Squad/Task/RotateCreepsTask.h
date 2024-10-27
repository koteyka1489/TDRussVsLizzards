// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Squad/Task/SquadBaseTask.h"
#include "RotateCreepsTask.generated.h"

/**
 * 
 */
UCLASS()
class TDRUSSVSLIZZARDS_API URotateCreepsTask : public USquadBaseTask
{
	GENERATED_BODY()

public:
    virtual void ExecuteTask() override;
};

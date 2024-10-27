// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SquadBaseTask.generated.h"

class ABaseSquadCreeps;

UCLASS(Abstract)
class TDRUSSVSLIZZARDS_API USquadBaseTask : public UObject
{
    GENERATED_BODY()

public:
    virtual void InitDestinationTask(FVector DestinationIn, ABaseSquadCreeps* Owner);
    virtual void ExecuteTask();

protected:
    FVector Destination = FVector::Zero();
    TObjectPtr<ABaseSquadCreeps> SquadOwner;
};

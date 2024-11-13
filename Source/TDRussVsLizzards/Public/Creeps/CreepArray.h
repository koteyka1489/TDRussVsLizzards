// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CreepArray.generated.h"

class ABaseCreepActor;

UCLASS()
class TDRUSSVSLIZZARDS_API UCreepArray : public UObject
{
    GENERATED_BODY()

public:

protected:

private:
    TMap<int32, TObjectPtr<ABaseCreepActor>> Creeps;

};

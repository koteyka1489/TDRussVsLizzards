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
    void Add(int32 Row, int32 Column, TObjectPtr<ABaseCreepActor> Creep);
    TArray<TObjectPtr<ABaseCreepActor>> GetColumnCreeps(int32 Column);
    TArray<TObjectPtr<ABaseCreepActor>> GetRowCreeps(int32 Row);

protected:

private:
    TMap<int32, TObjectPtr<ABaseCreepActor>> CreepsMap;


    int32 GenerateKey(int32 Row, int32 Column);
};

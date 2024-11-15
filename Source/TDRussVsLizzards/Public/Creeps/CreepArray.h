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
    void Add(int32 Key, TObjectPtr<ABaseCreepActor> Creep);
    static int32 GenerateKey(int32 Row, int32 Column);
    TArray<TObjectPtr<ABaseCreepActor>> GetColumnCreeps(int32 Column);
    TArray<TObjectPtr<ABaseCreepActor>> GetRowCreeps(int32 Row);

    TObjectPtr<ABaseCreepActor> GetValue(int32 Key) { return CreepsMap[Key]; }
    int32 Num() { return CreepsMap.Num(); }
    bool Contains(int32 Key) { return CreepsMap.Contains(Key); }
    int32 Remove(int32 Key) { return CreepsMap.Remove(Key); }

   // auto begin() { return CreepsMap.CreateIterator(); }
    auto begin() { return CreepsMap.begin(); }
    auto end() { return CreepsMap.end(); }

    //auto begin() const { return CreepsMap.CreateConstIterator(); }
    auto begin() const { return CreepsMap.begin(); }
    auto end() const { return CreepsMap.end(); }


protected:

private:
    TMap<int32, TObjectPtr<ABaseCreepActor>> CreepsMap;


    
};

// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
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
    int32 Num() const { return CreepsMap.Num(); }
    bool Contains(int32 Key) const { return CreepsMap.Contains(Key); }
    int32 Remove(int32 Key) { return CreepsMap.Remove(Key); }
    void Empty() { CreepsMap.Empty(); }
    auto Array() const{return CreepsMap.Array();}
    auto begin() { return CreepsMap.begin(); }
    auto end() { return CreepsMap.end(); }

    auto begin() const { return CreepsMap.begin(); }
    auto end() const { return CreepsMap.end(); }

protected:

private:
    TMap<int32, TObjectPtr<ABaseCreepActor>> CreepsMap;


};
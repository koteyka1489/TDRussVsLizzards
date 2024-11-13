// TD Russ Vs Lizzards Game

#include "Creeps/CreepArray.h"

void UCreepArray::Add(int32 Row, int32 Column, TObjectPtr<ABaseCreepActor> Creep)
{
    int32 Key = GenerateKey(Row, Column);
    CreepsMap.Add(Key, Creep);
}

TArray<TObjectPtr<ABaseCreepActor>> UCreepArray::GetColumnCreeps(int32 Column)
{
    TArray<TObjectPtr<ABaseCreepActor>> Result;
    int32 Row = 0;
    while (CreepsMap.Contains(GenerateKey(Row, Column)))
    {
        Result.Add(CreepsMap[GenerateKey(Row, Column)]);
        Row++;
    }

    return Result;
}

TArray<TObjectPtr<ABaseCreepActor>> UCreepArray::GetRowCreeps(int32 Row)
{
    TArray<TObjectPtr<ABaseCreepActor>> Result;
    int32 Column = 0;

    while (CreepsMap.Contains(GenerateKey(Row, Column)))
    {
        Result.Add(CreepsMap[GenerateKey(Row, Column)]);
        Column++;
    }

    return Result;
}

int32 UCreepArray::GenerateKey(int32 Row, int32 Column)
{
    return Row * 100 + Column;
}

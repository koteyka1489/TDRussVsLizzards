// TD Russ Vs Lizzards Game

#include "Creeps/CreepArray.h"
#include "Algo/Reverse.h"

void UCreepArray::Add(int32 Row, int32 Column, TObjectPtr<ABaseCreepActor> Creep)
{
    int32 Key = GenerateKey(Row, Column);
    CreepsMap.Add(Key, Creep);
}

void UCreepArray::Add(int32 Key, TObjectPtr<ABaseCreepActor> Creep) 
{
    CreepsMap.Add(Key, Creep);
}

void UCreepArray::ReverseKeysMap()
{
    TArray<int32> KeyArray;
    for (auto& Creep : CreepsMap)
    {
        KeyArray.Add(Creep.Key);
    }
    Algo::Reverse(KeyArray);

    int32 Index = 0;
    for (auto& Creep : CreepsMap)
    {
        Creep.Key = KeyArray[Index];
        Index++;
    }
}


void UCreepArray::ReverseValuesMap()
{
    TArray<TObjectPtr<ABaseCreepActor>> CreepsPtrArray;

    for (auto& Creep : CreepsMap)
    {
        CreepsPtrArray.Add(Creep.Value);
    }
    Algo::Reverse(CreepsPtrArray);

    int32 Index = 0;
    for (auto& Creep : CreepsMap)
    {
        Creep.Value = CreepsPtrArray[Index];
        Index++;
    }
}

int32 UCreepArray::GenerateKey(int32 Row, int32 Column)
{
    return Row * 100 + Column;
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

// TD Russ Vs Lizzards Game


#include "Squad/SquadsGroup.h"
#include "Squad/BaseSquadCreeps.h"

USquadsGroup::USquadsGroup()
{
}

void USquadsGroup::AddSquad(const TObjectPtr<ABaseSquadCreeps>& Squad)
{
    if (!GroupedSquads.Contains(Squad))
    {
        GroupedSquads.Add(Squad);
    }

    CalculateBaseGroupLocations();
}

void USquadsGroup::CalculateBaseGroupLocations()
{
    
}
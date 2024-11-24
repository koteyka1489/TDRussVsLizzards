// TD Russ Vs Lizzards Game


#include "Squad/SquadsGroup.h"
#include "Squad/BaseSquadCreeps.h"
#include "DrawDebugHelpers.h"

USquadsGroup::USquadsGroup()
{
}

void USquadsGroup::AddSquad(const TArray<TObjectPtr<ABaseSquadCreeps>>& ChoisenSquads)
{
    for (int i = 0; i < ChoisenSquads.Num(); i++)
    {
        if (!GroupedSquads.Contains(ChoisenSquads[i]))
        {
            GroupedSquads.Add(ChoisenSquads[i]);
        }
    }

    CalculateBaseGroupLocations();
    CalculateSquadRotationsAndLocationsFromCenter();
}

void USquadsGroup::CalculateBaseGroupLocations()
{
    FVector SumAllLocations = FVector::ZeroVector;
    for (const auto& Squad : GroupedSquads)
    {
        SumAllLocations += Squad->GetActorLocation();
    }
    CenterLocationGroup = SumAllLocations / GroupedSquads.Num();

    DrawDebugLine(GetWorld(), CenterLocationGroup, CenterLocationGroup + FVector(0.0, 0.0, 3000.0), FColor::Red, false, 10.0f, 0u, 20.f);
}

void USquadsGroup::CalculateSquadRotationsAndLocationsFromCenter()
{
    for (const auto& Squad : GroupedSquads)
    {
        SquadGroupLocationsFromCenter.Add(CenterLocationGroup - Squad->GetActorLocation());
        SquadGroupRotationsFromCenter.Add(Squad->GetActorQuat());
    }
}
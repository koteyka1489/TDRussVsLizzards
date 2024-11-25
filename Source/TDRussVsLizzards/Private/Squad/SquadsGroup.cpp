// TD Russ Vs Lizzards Game


#include "Squad/SquadsGroup.h"
#include "Squad/BaseSquadCreeps.h"
#include "DrawDebugHelpers.h"

USquadsGroup::USquadsGroup()
{
}

void USquadsGroup::AddSquad(const TArray<TObjectPtr<ABaseSquadCreeps>>& ChoisenSquads)
{
    GroupedSquads.Empty();
    GroupedSquads = ChoisenSquads;

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
        FVector VecToSquad = Squad->GetRightCornerCreepLocation() - CenterLocationGroup;
        SquadGroupRightCornerLocationsFromCenter.Add(VecToSquad);
        SquadGroupRotationsFromCenter.Add(Squad->GetActorForwardVector());

        DrawDebugLine(GetWorld(), CenterLocationGroup, CenterLocationGroup + VecToSquad, FColor::Red, false, 10.0f, 0u, 20.f);
    }
}
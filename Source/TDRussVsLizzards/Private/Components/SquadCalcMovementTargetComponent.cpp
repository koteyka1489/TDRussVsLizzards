// TD Russ Vs Lizzards Game

#include "Components/SquadCalcMovementTargetComponent.h"
#include "Creeps/BaseCreepActor.h"
#include "Squad/BaseSquadCreeps.h"

USquadCalcMovementTargetComponent::USquadCalcMovementTargetComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void USquadCalcMovementTargetComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerSquad = Cast<ABaseSquadCreeps>(GetOwner());
    checkf(IsValid(OwnerSquad), TEXT("Get Owner Squad is Failed"));
    CreepsArray = OwnerSquad->GetCreeps();
}

void USquadCalcMovementTargetComponent::UpdateNewCreepsPositions(
    int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor)
{
    int32 Width      = NewWidth;
    int32 Heigth     = OwnerSquad->GetCreepsNum() / Width;
    NewSquadRotation = NewSquadForwardVerctor.Rotation();

    NewCreepsLocations = OwnerSquad->CalculateCreepsPositions(0, Heigth, 0, Width, NewStartCreepSpawnLocation, NewSquadForwardVerctor);

    int32 CreepsShortage = OwnerSquad->GetCreepsNum() - Heigth * Width;
    if (CreepsShortage > 0)
    {
        int32 StartSpawnRemainderCreeps = Width / 2 - CreepsShortage / 2;
        NewCreepsLocations.Append(OwnerSquad->CalculateCreepsPositions(Heigth, Heigth + 1, StartSpawnRemainderCreeps,
            StartSpawnRemainderCreeps + CreepsShortage, NewStartCreepSpawnLocation, NewSquadForwardVerctor));
    }
}

void USquadCalcMovementTargetComponent::SetCreepsMovingDestination()
{
    int32 Index = 0;
    for (const auto& Creep : *CreepsArray)
    {
        Creep->SetCreepMovingDestination(NewCreepsLocations[Index]);
        Creep->SetCreepPostMovingRotation(NewSquadRotation);
        Index++;
    }
}

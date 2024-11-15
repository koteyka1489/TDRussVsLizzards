// TD Russ Vs Lizzards Game

#include "Components/SquadMovementComponent.h"
#include "Creeps/BaseCreepActor.h"
#include "Squad/BaseSquadCreeps.h"
#include "Creeps/CreepArray.h"

class ABaseSquadCreeps;

USquadMovementComponent::USquadMovementComponent()
{

    PrimaryComponentTick.bCanEverTick = true;
}

void USquadMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    auto OwnerSquad = Cast<ABaseSquadCreeps>(GetOwner());
    checkf(IsValid(OwnerSquad), TEXT("Get Owner Squad is Failed"));
    CreepsArray = OwnerSquad->GetCreeps();
}

void USquadMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bRotatingSquadIsContinue)
    {
        RotatingCreeps(DeltaTime);
    }

    if (bMovingSquadIsContinue && !bRotatingSquadIsContinue)
    {
        MovingCreeps(DeltaTime);
    }
}

void USquadMovementComponent::SetSquadMovement()
{
    bRotatingSquadIsContinue = true;
    bMovingSquadIsContinue   = true;
}

void USquadMovementComponent::StopAllMovings()
{
    bRotatingSquadIsContinue = false;
    bMovingSquadIsContinue   = false;

    for (const auto& Creep : *CreepsArray)
    {
        Creep.Value->StopMoving();
    }
}

void USquadMovementComponent::RotatingCreeps(float DeltaTime)
{
    int32 CreepEndRotatingCounter = 0;
    for (const auto& Creep : *CreepsArray)
    {
        if (Creep.Value->TickCreepRotating(DeltaTime))
        {
            CreepEndRotatingCounter++;
        }
    }

    if (CreepEndRotatingCounter == CreepsArray->Num())
    {
        bRotatingSquadIsContinue = false;
    }
}

void USquadMovementComponent::MovingCreeps(float DeltaTime)
{
    int32 CreepEndMovingCounter = 0;
    for (const auto& Creep : *CreepsArray)
    {
        if (Creep.Value->TickCreepMoving(DeltaTime))
        {
            CreepEndMovingCounter++;
        }
    }

    if (CreepEndMovingCounter == CreepsArray->Num())
    {
        bMovingSquadIsContinue = false;
    }
}

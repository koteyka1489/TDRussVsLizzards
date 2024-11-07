// TD Russ Vs Lizzards Game

#include "Components/SquadMovementComponent.h"
#include "Creeps/BaseCreepActor.h"
#include "Squad/BaseSquadCreeps.h"


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

    if (CurrentState == ESquadState::Idle) return;

    if (CurrentState == ESquadState::Rotating)
    {
        RotatingCreeps(DeltaTime);
    }

    if (CurrentState == ESquadState::Movement)
    {
        MovingCreeps(DeltaTime);
    }
    
}

void USquadMovementComponent::SetSquadMovement() 
{
    CurrentState = ESquadState::Rotating;
    TasksQueue.Enqueue(ESquadState::Movement);
}

void USquadMovementComponent::RotatingCreeps(float DeltaTime)
{

    for (const auto& Creep : *CreepsArray)
    {
        // Get speeds and vec dest
        // calculate
        // check creeps end rotaring and count
        //set rotating
    }
    
    // check count and end task
}

void USquadMovementComponent::MovingCreeps(float DeltaTime)
{

}

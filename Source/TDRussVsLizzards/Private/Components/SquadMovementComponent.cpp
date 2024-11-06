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
        RotatingCreeps();
    }

    if (CurrentState == ESquadState::Movement)
    {
        MovingCreeps();
    }
    
}

void USquadMovementComponent::SetSquadMovement() 
{
    CurrentState = ESquadState::Rotating;
    TasksQueue.Enqueue(ESquadState::Movement);
}

void USquadMovementComponent::RotatingCreeps() 
{

}

void USquadMovementComponent::MovingCreeps() 
{

}

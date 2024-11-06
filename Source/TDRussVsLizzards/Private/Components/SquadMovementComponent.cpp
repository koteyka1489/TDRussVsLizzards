// TD Russ Vs Lizzards Game

#include "Components/SquadMovementComponent.h"

USquadMovementComponent::USquadMovementComponent()
{

    PrimaryComponentTick.bCanEverTick = true;
}

void USquadMovementComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USquadMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// TD Russ Vs Lizzards Game

#include "Components/SquadCalcMovementTargetComponent.h"

USquadCalcMovementTargetComponent::USquadCalcMovementTargetComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void USquadCalcMovementTargetComponent::BeginPlay()
{
    Super::BeginPlay();
}

// TD Russ Vs Lizzards Game

#include "Components/CreepMovementComponent.h"

UCreepMovementComponent::UCreepMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCreepMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    CreepCurrentSpeeds.SpeedMoving   = 0.0f;
    CreepCurrentSpeeds.SpeedRotating = 0.0f;
}

void UCreepMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCreepMovementComponent::SetCreepSpeeds(float SpeedRotatingIn, float SpeedMovingIn)
{
    CreepMaxSpeeds.SpeedMoving   = SpeedMovingIn;
    CreepMaxSpeeds.SpeedRotating = SpeedRotatingIn;
}

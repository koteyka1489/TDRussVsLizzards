// TD Russ Vs Lizzards Game

#include "Components/CreepMovementComponent.h"
#include "Creeps/BaseCreepActor.h"

UCreepMovementComponent::UCreepMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCreepMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    CreepCurrentSpeeds.SpeedMoving   = 0.0f;
    CreepCurrentSpeeds.SpeedRotating = 0.0f;

    OwnerCreep = Cast<ABaseCreepActor>(GetOwner());
    check(IsValid(OwnerCreep));
}

bool UCreepMovementComponent::TickCreepMoving(float& DeltaTime)
{
    FVector VecToDestination = MovingDestination - OwnerCreep->GetActorLocation();
    if (VecToDestination.SizeSquared() <= DistSquaredEndMove)
    {
        CreepMovementState = ECreepMovementState::idle;
        return true;
    }

    FVector Offset = VecToDestination.GetSafeNormal2D() * CreepCurrentSpeeds.SpeedMoving * DeltaTime;
    OwnerCreep->SetActorLocation(OwnerCreep->GetActorLocation() + Offset);


    if (VecToDestination.SizeSquared() <= DistSquaredStopingMoving)
    {
        CreepMovementState = ECreepMovementState::StopingMoving;
    }
    UpdateMovingSpeed(DeltaTime);

    return false;
}

void UCreepMovementComponent::SetMovingDestination(FVector MovingDestinationIn)
{
    MovingDestination              = MovingDestinationIn;
    CreepCurrentSpeeds.SpeedMoving = 10.0f;
    CreepMovementState             = ECreepMovementState::StartingMoving;
}

void UCreepMovementComponent::SetCreepMaxSpeeds(float SpeedRotatingIn, float SpeedMovingIn)
{
    CreepMaxSpeeds.SpeedMoving   = SpeedMovingIn;
    CreepMaxSpeeds.SpeedRotating = SpeedRotatingIn;
}

void UCreepMovementComponent::UpdateMovingSpeed(float& DeltaTime)
{
    if (CreepMovementState == ECreepMovementState::idle || CreepMovementState == ECreepMovementState::Moving) return;

    if (CreepMovementState == ECreepMovementState::StartingMoving)
    {
        CreepCurrentSpeeds.SpeedMoving =
            FMath::FInterpConstantTo(CreepCurrentSpeeds.SpeedMoving, CreepMaxSpeeds.SpeedMoving, DeltaTime, MoveInterpSpeed);
        if (FMath::IsNearlyEqual(CreepCurrentSpeeds.SpeedMoving, CreepMaxSpeeds.SpeedMoving, 3.0f))
        {
            CreepMovementState = ECreepMovementState::Moving;
        }
    }

    if (CreepMovementState == ECreepMovementState::StopingMoving)
    {
        CreepCurrentSpeeds.SpeedMoving =
            FMath::FInterpConstantTo(CreepCurrentSpeeds.SpeedMoving, 10.0, DeltaTime, MoveInterpSpeed);
    }
   
}

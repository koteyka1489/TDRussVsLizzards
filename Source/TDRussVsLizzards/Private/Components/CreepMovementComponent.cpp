// TD Russ Vs Lizzards Game

#include "Components/CreepMovementComponent.h"
#include "Creeps/BaseCreepActor.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UCreepMovementComponent::UCreepMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCreepMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    CreepCurrentSpeeds.SpeedMoving = 0.0f;
    CreepCurrentSpeeds.SpeedRotating = CreepMaxSpeeds.SpeedRotating;

    OwnerCreep = Cast<ABaseCreepActor>(GetOwner());
    check(IsValid(OwnerCreep));

    CreepMaxSpeeds.SpeedMoving =
        CreepMaxSpeeds.SpeedMoving + FMath::FRandRange(-CreepSpeedRandoms.MovingRandom, CreepSpeedRandoms.MovingRandom);

    CreepMaxSpeeds.SpeedRotating =
        CreepMaxSpeeds.SpeedRotating + FMath::FRandRange(-CreepSpeedRandoms.RotatingRandom, CreepSpeedRandoms.RotatingRandom);

    CreepSpeedRandoms.MoveInterpSpeed = CreepSpeedRandoms.MoveInterpSpeed +
                                        FMath::FRandRange(-CreepSpeedRandoms.MoveInterpSpeedRand, CreepSpeedRandoms.MoveInterpSpeedRand);
}

bool UCreepMovementComponent::TickCreepMoving(float DeltaTime)
{
    FVector VecToDestination = MovingDestination - OwnerCreep->GetActorLocation();
    if (VecToDestination.SizeSquared() <= DistSquaredEndMove)
    {
        if (CreepMovementState != ECreepMovementState::idle)
        {
            CreepMovementState = ECreepMovementState::idle;
            CreepCurrentSpeeds.SpeedMoving = 0.0f;

            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle, this, &UCreepMovementComponent::PostMovingRotation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), true);
        }

        return true;
    }

    FVector Offset = VecToDestination.GetSafeNormal2D() * CreepCurrentSpeeds.SpeedMoving * DeltaTime;
    OwnerCreep->SetActorLocation(OwnerCreep->GetActorLocation() + Offset);

    UpdateMovingSpeed(DeltaTime);

    return false;
}

bool UCreepMovementComponent::TickCreepRotating(float DeltaTime, bool PostMoveRotation)
{
    FQuat CreepQuat = OwnerCreep->GetActorQuat();
    FVector Direction = (MovingDestination - OwnerCreep->GetActorLocation()).GetSafeNormal2D();
    //FRotator TargetRotator = FRotationMatrix::MakeFromX(Direction).Rotator();
    FQuat TargetRotationQuat = PostMoveRotation ? NewRotationQuat : Direction.Rotation().Quaternion();

    if (CreepQuat.Equals(TargetRotationQuat))
    {
        if (PostMoveRotation) GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        return true;
    }

    FQuat NewRotation = FMath::QInterpConstantTo(CreepQuat, TargetRotationQuat, DeltaTime, CreepMaxSpeeds.SpeedRotating);
    OwnerCreep->SetActorRotation(NewRotation);

    return false;
}

void UCreepMovementComponent::StopMoving()
{
    CreepMovementState = ECreepMovementState::idle;
    CreepCurrentSpeeds.SpeedMoving = 0.0f;
}

void UCreepMovementComponent::SetMovingDestination(FVector MovingDestinationIn)
{
    MovingDestination = MovingDestinationIn;
    if (CreepMovementState == ECreepMovementState::idle)
    {
        CreepCurrentSpeeds.SpeedMoving = 100.0f;
        CreepMovementState = ECreepMovementState::StartingMoving;
    }

}

void UCreepMovementComponent::SetCreepPostMovingRotation(FVector NewSquadForwardVectorIn)
{
    NewSquadForwardVector = NewSquadForwardVectorIn;
    NewRotationQuat = NewSquadForwardVectorIn.Rotation().Quaternion();
}

void UCreepMovementComponent::UpdateMovingSpeed(float DeltaTime)
{
    if (CreepMovementState == ECreepMovementState::idle || CreepMovementState == ECreepMovementState::Moving) return;

    if (CreepMovementState == ECreepMovementState::StartingMoving)
    {
        CreepCurrentSpeeds.SpeedMoving = FMath::FInterpConstantTo(
            CreepCurrentSpeeds.SpeedMoving, CreepMaxSpeeds.SpeedMoving, DeltaTime, CreepSpeedRandoms.MoveInterpSpeed);
        if (FMath::IsNearlyEqual(CreepCurrentSpeeds.SpeedMoving, CreepMaxSpeeds.SpeedMoving, 3.0f))
        {
            CreepMovementState = ECreepMovementState::Moving;
        }
    }
}

void UCreepMovementComponent::PostMovingRotation()
{
    double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
    TickCreepRotating(DeltaTime, true);
}
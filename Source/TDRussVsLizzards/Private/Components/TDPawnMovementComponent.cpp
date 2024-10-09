// TD Russ Vs Lizzards Game

#include "Components/TDPawnMovementComponent.h"
#include "Creeps/BaseCreepPawn.h"
#include "DrawDebugHelpers.h"

UTDPawnMovementComponent::UTDPawnMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTDPawnMovementComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTDPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bDestinationToRotatingIsSet)
    {
        RotatingToLocation(DeltaTime);
    }
    if (bDestinationToMovingIsSet)
    {
        MovingToLocation(DeltaTime);
    }
}

void UTDPawnMovementComponent::MoveToLocation(FVector Location)
{
    DestinationToMoving       = Location;
    bDestinationToMovingIsSet = true;
}

void UTDPawnMovementComponent::RotateToLocation(FVector Location)
{
    DestinationToRotating       = Location;
    bDestinationToRotatingIsSet = true;
}

void UTDPawnMovementComponent::MovingToLocation(float DeltaTime)
{
    auto OwnerPawn = Cast<ABaseCreepPawn>(GetOwner());
    if (OwnerPawn)
    {
        FVector VecToDestination = DestinationToMoving - OwnerPawn->GetActorLocation();
        if (VecToDestination.Length() <= 10)
        {
            bDestinationToMovingIsSet = false;
            return;
        }
        else
        {
            FVector Direction   = VecToDestination.GetSafeNormal();
            Direction.Z         = 0.0f;
            FVector Offset      = Direction * SpeedMoving * DeltaTime;
            FVector NewLocation = OwnerPawn->GetActorLocation() + Offset;
            OwnerPawn->SetActorLocation(NewLocation);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Cast to owner is Failed"));
        checkNoEntry();
    }
}

void UTDPawnMovementComponent::RotatingToLocation(float DeltaTime)
{
    auto OwnerPawn = Cast<ABaseCreepPawn>(GetOwner());
    if (OwnerPawn)
    {
        FVector VecToDestinationNormalize = (DestinationToRotating - OwnerPawn->GetActorLocation()).GetSafeNormal();
        FVector PawnForwardVec            = OwnerPawn->GetActorForwardVector();

        float DotPawnForwardToDestination = FVector::DotProduct(VecToDestinationNormalize, PawnForwardVec);

        if (DotPawnForwardToDestination >= 0.98)
        {
            bDestinationToRotatingIsSet = false;
            return;
        }
        else
        {
            FVector PawnRightVec            = OwnerPawn->GetActorRightVector();
            float DotPawnRightToDestination = FVector::DotProduct(VecToDestinationNormalize, PawnRightVec);

            FRotator OffsetRotation = FRotator{0.0f, SpeedRotating * DeltaTime, 0.0f};

            if (DotPawnRightToDestination >= 0)
            {
                FRotator NewRotation = OwnerPawn->GetActorRotation() + OffsetRotation;
                OwnerPawn->SetActorRotation(NewRotation);
            }
            else
            {
                FRotator NewRotation = OwnerPawn->GetActorRotation() - OffsetRotation;
                OwnerPawn->SetActorRotation(NewRotation);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Cast to owner is Failed"));
        checkNoEntry();
    }
}

// TD Russ Vs Lizzards Game

#include "Components/ActorMovementComponent.h"
#include "Creeps\BaseCreepActor.h"

UActorMovementComponent::UActorMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UActorMovementComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UActorMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!OwnerPawn)
    {
        OwnerPawn = Cast<ABaseCreepActor>(GetOwner());
    }

    if (bDestinationToRotatingIsSet)
    {
        RotatingToLocation(DeltaTime);
    }
    if (bDestinationToMovingIsSet)
    {
        MovingToLocation(DeltaTime);
    }
}

void UActorMovementComponent::InitializeComponent()
{
    Super::InitializeComponent();
    
}

void UActorMovementComponent::MoveToLocation(FVector Location)
{
    DestinationToMoving       = Location;
    bDestinationToMovingIsSet = true;
    if (bAutoOrientToMovement)
    {
        RotateToLocation(Location);
    }
}

void UActorMovementComponent::RotateToLocation(FVector Location)
{
    DestinationToRotating       = Location;
    bDestinationToRotatingIsSet = true;
}

void UActorMovementComponent::MovingToLocation(float DeltaTime)
{
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

void UActorMovementComponent::RotatingToLocation(float DeltaTime)
{
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

// TD Russ Vs Lizzards Game

#include "Components/TDPawnMovementComponent.h"
#include "Creeps/BaseCreepPawn.h"

UTDPawnMovementComponent::UTDPawnMovementComponent() {}

void UTDPawnMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    PrimaryComponentTick.bCanEverTick = true;
}

void UTDPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bDestinationIsSet)
    {
        MoveToLocation(DeltaTime);
    }
}

void UTDPawnMovementComponent::MoveToLocation(FVector Location)
{
    Destination       = Location;
    bDestinationIsSet = true;
}

void UTDPawnMovementComponent::MoveToLocation(float DeltaTime)
{
    auto OwnerPawn = Cast<ABaseCreepPawn>(GetOwner());
    if (OwnerPawn)
    {
        FVector VecToDestination = Destination - OwnerPawn->GetActorLocation();
        if (VecToDestination.Length() <= 10)
        {
            bDestinationIsSet = false;
            return;
        }
        else
        {
            FVector Direction   = VecToDestination.GetSafeNormal();
            FVector Offset      = Direction * Speed * DeltaTime;
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
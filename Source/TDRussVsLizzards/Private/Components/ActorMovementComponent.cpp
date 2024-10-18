// TD Russ Vs Lizzards Game

#include "Components/ActorMovementComponent.h"
#include "Squad/BaseSquadCreeps.h"
#include "Creeps/BaseCreepActor.h"

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

    if (bDestinationToRotatingIsSet)
    {
        RotatingToLocation(DeltaTime);
    }
    if (bDestinationToMovingIsSet)
    {
        MovingToLocation(DeltaTime);
    }
}

void UActorMovementComponent::MoveToLocation(FVector Location)
{
    DestinationToMoving       = Location;
    DestinationToMoving.Z     = 0.0;
    bDestinationToMovingIsSet = true;
    if (bAutoOrientToMovement)
    {
        RotateToLocation(Location);
    }
}

void UActorMovementComponent::RotateToLocation(FVector Location)
{
    DestinationToRotating       = Location;
    DestinationToRotating.Z     = 0.0;
    bDestinationToRotatingIsSet = true;
}

void UActorMovementComponent::MovingToLocation(float DeltaTime)
{
    auto OwnerSquad = Cast<ABaseSquadCreeps>(GetOwner());
    checkf(IsValid(OwnerSquad), TEXT("Get Owner Squad is Failed"));
    auto CreepsArray = OwnerSquad->GetCreeps();

    FVector VecToDestination = DestinationToMoving - CreepsArray[0]->GetActorLocation();
    VecToDestination.Z       = 0.0;


    if (VecToDestination.Length() <= 10)
    {
        bDestinationToMovingIsSet = false;
        checkf(OnMovingComplete.ExecuteIfBound(), TEXT("On moving Complete delegate not bound"));
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Moving Complete"));
        return;
    }
    else
    {
        FVector Direction = VecToDestination.GetSafeNormal();
        Direction.Z       = 0.0f;
        FVector Offset    = Direction * SpeedMoving * DeltaTime;

        for (auto& Creep : CreepsArray)
        {
            FVector NewLocation = Creep->GetActorLocation() + Offset;
            Creep->SetActorLocation(NewLocation);
        }

        FVector NewLocation = OwnerSquad->GetActorLocation() + Offset;
        OwnerSquad->SetActorLocation(NewLocation);
    }
}

void UActorMovementComponent::RotatingToLocation(float DeltaTime)
{
    auto OwnerSquad = Cast<ABaseSquadCreeps>(GetOwner());
    checkf(IsValid(OwnerSquad), TEXT("Get Owner Squad is Failed"));
    auto CreepsArray = OwnerSquad->GetCreeps();

    FVector VecToDestinationNormalize = (DestinationToRotating - CreepsArray[0]->GetActorLocation()).GetSafeNormal();
    VecToDestinationNormalize.Z       = 0.0;
    FVector CreepForwardVec            = CreepsArray[0]->GetActorForwardVector();

    float DotPawnForwardToDestination = FVector::DotProduct(VecToDestinationNormalize, CreepForwardVec);

    if (DotPawnForwardToDestination >= 0.99)
    {
        bDestinationToRotatingIsSet = false;
        return;
    }
    else
    {
        FVector CreepRightVec            = CreepsArray[0]->GetActorRightVector();
        float DotPawnRightToDestination = FVector::DotProduct(VecToDestinationNormalize, CreepRightVec);

        FRotator OffsetRotation = FRotator{0.0f, SpeedRotating * DeltaTime, 0.0f};

        if (DotPawnRightToDestination >= 0.0)
        {
            for (auto& Creep : CreepsArray)
            {
                FRotator NewRotation = Creep->GetActorRotation() + OffsetRotation;
                Creep->SetActorRotation(NewRotation);
            }
            FRotator NewRotation = OwnerSquad->GetActorRotation() + OffsetRotation;
            OwnerSquad->SetActorRotation(NewRotation);
        }
        else
        {
            for (auto& Creep : CreepsArray)
            {
                FRotator NewRotation = Creep->GetActorRotation() - OffsetRotation;
                Creep->SetActorRotation(NewRotation);
            }
            FRotator NewRotation = OwnerSquad->GetActorRotation() - OffsetRotation;
            OwnerSquad->SetActorRotation(NewRotation);
        }
    }
}

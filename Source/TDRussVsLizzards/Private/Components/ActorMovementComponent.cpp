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
    auto OwnerSquad = Cast<ABaseSquadCreeps>(GetOwner());
    checkf(IsValid(OwnerSquad), TEXT("Get Owner Squad is Failed"));
    auto CreepsArray = OwnerSquad->GetCreeps();

    FVector VecToDestination = DestinationToMoving - CreepsArray[0]->GetActorLocation();

    FString Message = FString::Printf(TEXT("Destination - %s"), *DestinationToMoving.ToString());
    GEngine->AddOnScreenDebugMessage(1, 0, FColor::Red, Message);

    FString Message2 = FString::Printf(TEXT("VecToDestination - %s"), *VecToDestination.ToString());
    GEngine->AddOnScreenDebugMessage(2, 0, FColor::Red, Message2);

    FString Message3 = FString::Printf(TEXT("VecToDestination lenghth - %f"), VecToDestination.Length());
    GEngine->AddOnScreenDebugMessage(3, 0, FColor::Red, Message3);

    if (VecToDestination.Length() <= 10)
    {
        bDestinationToMovingIsSet = false;
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
    FVector CreepForwardVec            = CreepsArray[0]->GetActorForwardVector();

    float DotPawnForwardToDestination = FVector::DotProduct(VecToDestinationNormalize, CreepForwardVec);

    if (DotPawnForwardToDestination >= 0.98)
    {
        bDestinationToRotatingIsSet = false;
        return;
    }
    else
    {
        FVector CreepRightVec            = CreepsArray[0]->GetActorRightVector();
        float DotPawnRightToDestination = FVector::DotProduct(VecToDestinationNormalize, CreepRightVec);

        FRotator OffsetRotation = FRotator{0.0f, SpeedRotating * DeltaTime, 0.0f};

        if (DotPawnRightToDestination >= 0)
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

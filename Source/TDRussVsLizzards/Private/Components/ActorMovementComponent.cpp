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

    OwnerSquad = Cast<ABaseSquadCreeps>(GetOwner());
    checkf(IsValid(OwnerSquad), TEXT("Get Owner Squad is Failed"));
    CreepsArray = OwnerSquad->GetCreeps();
}

void UActorMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bDestinationToSquadFrontRotationIsSet)
    {
        RotatingFrontSquadToLocation(DeltaTime);
    }
    // if (bDestinationToRotatingIsSet)
    //{
    //     RotatingToLocation(DeltaTime);
    // }
    // if (bDestinationToMovingIsSet)
    //{
    //     MovingToLocation(DeltaTime);
    // }
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

void UActorMovementComponent::RotateFrontSquadToLocation(FVector Location, const TArray<FVector>& CreepsLocationFromCenterSquadIn)
{
    CreepsLocationFromCenterSquad         = CreepsLocationFromCenterSquadIn;
    DestinationToSquadFronRotation        = Location;
    bDestinationToSquadFrontRotationIsSet = true;
    CalculateDestinationCreepsToRotateFrontSquad();
}

void UActorMovementComponent::RotateToLocation(FVector Location)
{
    DestinationToRotating       = Location;
    DestinationToRotating.Z     = 0.0;
    bDestinationToRotatingIsSet = true;
}

void UActorMovementComponent::MovingToLocation(float DeltaTime)
{

    FVector VecToDestination = DestinationToMoving - OwnerSquad->GetActorLocation();
    VecToDestination.Z       = 0.0;

    if (VecToDestination.Length() <= 10)
    {
        bDestinationToMovingIsSet = false;
        OnMovingComplete.ExecuteIfBound();
        return;
    }
    else
    {
        FVector Direction = VecToDestination.GetSafeNormal();
        Direction.Z       = 0.0f;
        FVector Offset    = Direction * SpeedMoving * DeltaTime;

        for (auto& Creep : *CreepsArray)
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

    FVector VecToDestinationNormalize = (DestinationToRotating - OwnerSquad->GetActorLocation()).GetSafeNormal();
    VecToDestinationNormalize.Z       = 0.0;
    FVector CreepForwardVec           = OwnerSquad->GetActorForwardVector();

    float DotPawnForwardToDestination = FVector::DotProduct(VecToDestinationNormalize, CreepForwardVec);

    if (DotPawnForwardToDestination >= 0.99)
    {
        bDestinationToRotatingIsSet = false;
        return;
    }
    else
    {
        FVector CreepRightVec           = OwnerSquad->GetActorRightVector();
        float DotPawnRightToDestination = FVector::DotProduct(VecToDestinationNormalize, CreepRightVec);

        FRotator OffsetRotation = FRotator{0.0f, SpeedRotating * DeltaTime, 0.0f};

        if (DotPawnRightToDestination >= 0.0)
        {
            for (auto& Creep : *CreepsArray)
            {
                FRotator NewRotation = Creep->GetActorRotation() + OffsetRotation;
                Creep->SetActorRotation(NewRotation);
            }
            FRotator NewRotation = OwnerSquad->GetActorRotation() + OffsetRotation;
            OwnerSquad->SetActorRotation(NewRotation);
        }
        else
        {
            for (auto& Creep : *CreepsArray)
            {
                FRotator NewRotation = Creep->GetActorRotation() - OffsetRotation;
                Creep->SetActorRotation(NewRotation);
            }
            FRotator NewRotation = OwnerSquad->GetActorRotation() - OffsetRotation;
            OwnerSquad->SetActorRotation(NewRotation);
        }
    }
}

void UActorMovementComponent::RotatingFrontSquadToLocation(float DeltaTime)
{
    if (CreepEndRotatFrontSquadCounter == CreepsArray->Num())
    {
        CreepEndRotatFrontSquadCounter = 0;
        DestinationCreepsToRotateFrontSquad.Empty();
        bDestinationToSquadFrontRotationIsSet = false;
    }

    float FrameSpeed = SpeedMoving * DeltaTime;
    int32 CreepIndex = 0;

    for (auto& Creep : *CreepsArray)
    {
        FVector VecToDestination = DestinationCreepsToRotateFrontSquad[CreepIndex] - Creep->GetActorLocation();
        CreepIndex++;

        if (VecToDestination.Length() <= 10.0)
        {
            CreepEndRotatFrontSquadCounter++;
        }
        else
        {
            FVector Direction   = VecToDestination.GetSafeNormal();
            FVector Offset      = FrameSpeed * Direction;
            FVector NewLocation = Creep->GetActorLocation() + Offset;
        }
    }
}

void UActorMovementComponent::CalculateDestinationCreepsToRotateFrontSquad()
{

    if (!CreepsArray) return;

    if (CreepsLocationFromCenterSquad.Num() == 0) return;

    if (!DestinationCreepsToRotateFrontSquad.IsEmpty())
    {
        DestinationCreepsToRotateFrontSquad.Empty();
    }

    int32 CreepIndex = 0;

    FVector VecSquadToRotationDestinationNormalize = (DestinationToSquadFronRotation - OwnerSquad->GetActorLocation()).GetSafeNormal();
    VecSquadToRotationDestinationNormalize.Z       = 0.0;
    FVector SquadForwardVector                     = OwnerSquad->GetActorForwardVector();

    FQuat BetweenQuat = FQuat::FindBetweenNormals(SquadForwardVector, VecSquadToRotationDestinationNormalize);

    for (auto& Creep : *CreepsArray)
    {
        FVector DestVec = BetweenQuat.RotateVector(CreepsLocationFromCenterSquad[CreepIndex]);
        DestinationCreepsToRotateFrontSquad.Add(DestVec);
        CreepIndex++;
    }
}

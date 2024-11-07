// TD Russ Vs Lizzards Game

#include "Components/ActorMovementComponent.h"
#include "Squad/BaseSquadCreeps.h"
#include "Creeps/BaseCreepActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"

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
        RotatingFrontSquadToLocationFromCenter(DeltaTime);
    }
    if (bDestinationToRotatingIsSet)
    {
        RotatingToLocationQuat(DeltaTime);
    }
    if (bDestinationToMovingIsSet && !bDestinationToSquadFrontRotationIsSet)
    {
        MovingToLocation(DeltaTime);
    }
    if (bRebuildSquadIsSet)
    {
        RebuildingSquad(DeltaTime);
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

void UActorMovementComponent::RotateFrontSquadToLocation(FVector Location)
{
    DestinationToSquadFrontRotation = Location;
    UpdateCreepsLocationFromSidesSquad();
    bDestinationToSquadFrontRotationIsSet = true;
    CalculateDestinationCreepsToRotateFrontSquad();

    if (bAutoOrientToFrontSquadRotation)
    {
        RotateToLocation(Location);
    }
}

void UActorMovementComponent::RotateFrontSquadToLocationFromSide(FVector Location, ERotateFrontSquadBySide SideIn)
{
    DestinationToSquadFrontRotation = Location;
    UpdateCreepsLocationFromSidesSquad();
    bDestinationToSquadFrontRotationIsSet = true;
    CalculateDestinationCreepsToRotateFrontSquadBySide(SideIn);

    if (bAutoOrientToFrontSquadRotation)
    {
        RotateToLocation(Location);
    }
}

void UActorMovementComponent::StopAllMovings()
{
    bDestinationToMovingIsSet             = false;
    bDestinationToRotatingIsSet           = false;
    bDestinationToSquadFrontRotationIsSet = false;
    bRebuildSquadIsSet                    = false;
    DestinationCreepsToRotateFrontSquadFromCenter.Empty();
    CreepEndRotatFrontSquadCounter = 0;
    UpdateCreepsLocationFromSidesSquad();
    OnMovingComplete.ExecuteIfBound();
}

void UActorMovementComponent::RebuildSquad(const TArray<FVector>& NewCreepLocations)
{
    if (!bRebuildSquadIsSet)
    {
        bRebuildSquadIsSet = true;
        DestinationCreepsToRebuild.Append(NewCreepLocations);
        if (bAutoOrientToMovement)
        {
            RotateToLocation(NewCreepLocations[NewCreepLocations.Num() -1]);
        }
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

void UActorMovementComponent::RotatingToLocationQuat(float DeltaTime)
{
    FQuat SquadQuat   = OwnerSquad->GetActorQuat();
    FVector Direction = (DestinationToRotating - OwnerSquad->GetActorLocation()).GetSafeNormal2D();

    FRotator TargetRotator   = FRotationMatrix::MakeFromX(Direction).Rotator();
    FQuat TargetRotationQuat = TargetRotator.Quaternion();

    if (SquadQuat.Equals(TargetRotationQuat, KINDA_SMALL_NUMBER))
    {
        bDestinationToRotatingIsSet = false;
        OnRotatingCreepsComplete.ExecuteIfBound();
        return;
    }

    FQuat NewRotation = FMath::QInterpConstantTo(SquadQuat, TargetRotationQuat, DeltaTime, InterpSpeed);

    for (const auto& Creep : *CreepsArray)
    {
        Creep->SetActorRotation(NewRotation);
    }
    OwnerSquad->SetActorRotation(NewRotation);
}

void UActorMovementComponent::RotatingFrontSquadToLocationFromCenter(float DeltaTime)
{

    float FrameSpeed               = SpeedMoving * DeltaTime;
    int32 CreepIndex               = 0;
    CreepEndRotatFrontSquadCounter = 0;

    for (const auto& Creep : *CreepsArray)
    {
        FVector VecToDestination = DestinationCreepsToRotateFrontSquadFromCenter[CreepIndex] - Creep->GetActorLocation();

        if (VecToDestination.Length() <= 5.0)
        {
            CreepEndRotatFrontSquadCounter++;
        }
        else
        {
            FVector Direction   = VecToDestination.GetSafeNormal();
            FVector Offset      = FrameSpeed * Direction;
            FVector NewLocation = Creep->GetActorLocation() + Offset;
            Creep->SetActorLocation(NewLocation);
        }

        CreepIndex++;
    }

    FVector VecToDestination = DestinationCreepsToRotateFrontSquadFromCenter[CreepIndex] - OwnerSquad->GetActorLocation();
    FVector Direction        = VecToDestination.GetSafeNormal();
    FVector Offset           = FrameSpeed * Direction;
    FVector NewLocation      = OwnerSquad->GetActorLocation() + Offset;
    OwnerSquad->SetActorLocation(NewLocation);

    if (CreepsArray->Num() == CreepEndRotatFrontSquadCounter)
    {
        bDestinationToSquadFrontRotationIsSet = false;
        DestinationCreepsToRotateFrontSquadFromCenter.Empty();
        CreepEndRotatFrontSquadCounter = 0;
        UpdateCreepsLocationFromSidesSquad();
        if (!OnRotatingFrontSquadComplete.ExecuteIfBound())
        {
            check(0);
        }
    }
}

void UActorMovementComponent::RebuildingSquad(float DeltaTime)
{

    float FrameSpeed          = SpeedMoving * DeltaTime;
    int32 CreepIndex          = 0;
    CreepEndRebuildingCounter = 0;

    for (const auto& Creep : *CreepsArray)
    {
        FVector VecToDestination = DestinationCreepsToRebuild[CreepIndex] - Creep->GetActorLocation();

        if (VecToDestination.Length() <= 5.0)
        {
            Creep->PlayAnimationIdle();
            CreepEndRebuildingCounter++;
        }
        else
        {
            FVector Direction   = VecToDestination.GetSafeNormal();
            FVector Offset      = FrameSpeed * Direction;
            FVector NewLocation = Creep->GetActorLocation() + Offset;
            Creep->SetActorLocation(NewLocation);
        }

        CreepIndex++;
    }

    FVector VecToDestination = DestinationCreepsToRebuild[CreepIndex] - OwnerSquad->GetActorLocation();
    FVector Direction        = VecToDestination.GetSafeNormal();
    FVector Offset           = FrameSpeed * Direction;
    FVector NewLocation      = OwnerSquad->GetActorLocation() + Offset;
    OwnerSquad->SetActorLocation(NewLocation);

    if (CreepEndRebuildingCounter == CreepsArray->Num())
    {
        bRebuildSquadIsSet = false;
        DestinationCreepsToRebuild.Empty();
        CreepEndRebuildingCounter = 0;
        UpdateCreepsLocationFromSidesSquad();
        if (!OnRebuildingSquadComplete.ExecuteIfBound())
        {
            check(0);
        }
    }
}

void UActorMovementComponent::CalculateDestinationCreepsToRotateFrontSquad()
{

    if (!CreepsArray) return;

    if (CreepsLocationFromCenterSquad.Num() == 0) return;

    if (!DestinationCreepsToRotateFrontSquadFromCenter.IsEmpty())
    {
        DestinationCreepsToRotateFrontSquadFromCenter.Empty();
    }

    int32 CreepIndex = 0;

    FVector VecSquadToRotationDestinationNormalize = (DestinationToSquadFrontRotation - OwnerSquad->GetActorLocation()).GetSafeNormal();
    VecSquadToRotationDestinationNormalize.Z       = 0.0;
    FVector SquadForwardVector                     = OwnerSquad->GetActorForwardVector();

    FQuat BetweenQuat          = FQuat::FindBetweenNormals(SquadForwardVector, VecSquadToRotationDestinationNormalize);
    FQuat BetweenQuatNormalize = BetweenQuat.GetNormalized();
    for (auto& Creep : *CreepsArray)
    {
        FVector DestVec = BetweenQuatNormalize.RotateVector(CreepsLocationFromCenterSquad[CreepIndex]);

        DestinationCreepsToRotateFrontSquadFromCenter.Add(OwnerSquad->GetActorLocation() + DestVec);

        CreepIndex++;
    }
}

void UActorMovementComponent::CalculateDestinationCreepsToRotateFrontSquadBySide(ERotateFrontSquadBySide Side)
{
    if (!CreepsArray) return;

    if (!DestinationCreepsToRotateFrontSquadFromCenter.IsEmpty())
    {
        DestinationCreepsToRotateFrontSquadFromCenter.Empty();
    }
    FVector BaseRotatingLocation               = FVector::Zero();
    TArray<FVector>& CreepsLocationFromSidePtr = CreepsLocationFromCenterSquad;

    switch (Side)
    {
        case ERotateFrontSquadBySide::LeftCorner:
        {
            BaseRotatingLocation      = OwnerSquad->GetLeftCornerCreepLocation();
            CreepsLocationFromSidePtr = CreepsLocationFromLeftCornerSquad;
            break;
        }
        case ERotateFrontSquadBySide::RightCorner:
        {
            BaseRotatingLocation      = OwnerSquad->GetRightCornerCreepLocation();
            CreepsLocationFromSidePtr = CreepsLocationFromRightCornerSquad;
            break;
        }
        case ERotateFrontSquadBySide::Center:
        {
            BaseRotatingLocation      = OwnerSquad->GetActorLocation();
            CreepsLocationFromSidePtr = CreepsLocationFromCenterSquad;
            break;
        }

        default: break;
    }

    int32 CreepIndex = 0;

    FVector VecSquadToRotationDestinationNormalize = (DestinationToSquadFrontRotation - OwnerSquad->GetActorLocation()).GetSafeNormal();
    VecSquadToRotationDestinationNormalize.Z       = 0.0;
    FVector SquadForwardVector                     = OwnerSquad->GetActorForwardVector();

    FQuat BetweenQuat = FQuat::FindBetweenNormals(SquadForwardVector, VecSquadToRotationDestinationNormalize);
    // FQuat BetweenQuatNormalize = BetweenQuat.GetNormalized();
    for (const auto& Creep : *CreepsArray)
    {
        FVector DestVec = BetweenQuat.RotateVector(CreepsLocationFromSidePtr[CreepIndex]);

        DestinationCreepsToRotateFrontSquadFromCenter.Add(BaseRotatingLocation + DestVec);

        CreepIndex++;
    }

    FVector DestVec = BetweenQuat.RotateVector(CreepsLocationFromSidePtr[CreepIndex]);

    DestinationCreepsToRotateFrontSquadFromCenter.Add(BaseRotatingLocation + DestVec);
}

void UActorMovementComponent::UpdateCreepsLocationFromSidesSquad()
{
    if (CreepsArray->Num() == 0) return;
    FVector SquadLocation = OwnerSquad->GetActorLocation();

    CreepsLocationFromCenterSquad.Empty();
    CreepsLocationFromRightCornerSquad.Empty();
    CreepsLocationFromLeftCornerSquad.Empty();

    FVector RightCornerCreepLocation = OwnerSquad->GetRightCornerCreepLocation();
    FVector LeftCornerCreepLocation  = OwnerSquad->GetLeftCornerCreepLocation();

    for (const auto& Creep : *CreepsArray)
    {
        CreepsLocationFromCenterSquad.Add(Creep->GetActorLocation() - SquadLocation);
        CreepsLocationFromRightCornerSquad.Add(Creep->GetActorLocation() - RightCornerCreepLocation);
        CreepsLocationFromLeftCornerSquad.Add(Creep->GetActorLocation() - LeftCornerCreepLocation);
    }
    CreepsLocationFromCenterSquad.Add(OwnerSquad->GetActorLocation() - SquadLocation);
    CreepsLocationFromRightCornerSquad.Add(OwnerSquad->GetActorLocation() - RightCornerCreepLocation);
    CreepsLocationFromLeftCornerSquad.Add(OwnerSquad->GetActorLocation() - LeftCornerCreepLocation);
}

// TD Russ Vs Lizzards Game

#include "Components/CreepMovementComponent.h"
#include "Creeps/BaseCreepActor.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

UCreepMovementComponent::UCreepMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCreepMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    CreepCurrentSpeeds.SpeedMoving   = 0.0f;
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

bool UCreepMovementComponent::TickCreepMoving(float& DeltaTime)
{
    FVector VecToDestination = MovingDestination - OwnerCreep->GetActorLocation();
    if (VecToDestination.SizeSquared() <= DistSquaredEndMove)
    {
        if (CreepMovementState != ECreepMovementState::idle)
        {
            CreepMovementState             = ECreepMovementState::idle;
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

bool UCreepMovementComponent::TickCreepRotating(float& DeltaTime)
{
    FQuat SquadQuat   = OwnerCreep->GetActorQuat();
    FVector Direction = (MovingDestination - OwnerCreep->GetActorLocation()).GetSafeNormal2D();

    FRotator TargetRotator   = FRotationMatrix::MakeFromX(Direction).Rotator();
    FQuat TargetRotationQuat = TargetRotator.Quaternion();

    if (SquadQuat.Equals(TargetRotationQuat, KINDA_SMALL_NUMBER))
    {
        return true;
    }

    FQuat NewRotation = FMath::QInterpConstantTo(SquadQuat, TargetRotationQuat, DeltaTime, CreepMaxSpeeds.SpeedRotating);
    OwnerCreep->SetActorRotation(NewRotation);

    return false;
}

void UCreepMovementComponent::StopMoving() 
{
    CreepMovementState             = ECreepMovementState::idle;
    CreepCurrentSpeeds.SpeedMoving = 0.0f;
}

void UCreepMovementComponent::SetMovingDestination(FVector MovingDestinationIn)
{
    MovingDestination              = MovingDestinationIn;
    if (CreepMovementState == ECreepMovementState::idle)
    {
        CreepCurrentSpeeds.SpeedMoving = 100.0f;
        CreepMovementState             = ECreepMovementState::StartingMoving;
    }
    
}

void UCreepMovementComponent::SetCreepPostMovingRotation(FRotator NewSquadRotationIn)
{
    NewSquadRotation = NewSquadRotationIn;
}

void UCreepMovementComponent::UpdateMovingSpeed(float& DeltaTime)
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
    // const double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
    //
    // if (FMath::Abs((OwnerCreep->GetActorRotation().Yaw - NewSquadRotation.Yaw)) < 5.0)
    // {
    //     GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    // }
    // FRotator NewRotation = FMath::RInterpConstantTo(OwnerCreep->GetActorRotation(), NewSquadRotation, DeltaTime, 200.0);
    // OwnerCreep->SetActorRotation(NewRotation);
    
    const double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());  
    const FRotator CurrentRotation = OwnerCreep->GetActorRotation();  
    
    // Оптимизированная проверка близости углов  
    float AngleDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, NewSquadRotation.Yaw));  
    
    // Более точная интерполяция  
    FRotator NewRotation = FMath::RInterpTo(  
        CurrentRotation,   
        NewSquadRotation,   
        DeltaTime,   
        10.0f  // Плавность поворота  
    );  

    // Точная остановка при достижении цели  
    if (AngleDifference < 5.0f)  
    {  
        NewRotation = NewSquadRotation;  
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);  
    }  

    OwnerCreep->SetActorRotation(NewRotation);  
}

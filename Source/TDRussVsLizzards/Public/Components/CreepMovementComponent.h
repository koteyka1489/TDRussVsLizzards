// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CreepMovementComponent.generated.h"

class ABaseCreepActor;

enum class ECreepMovementState
{
    idle,
    StartingMoving,
    Moving,
    StopingMoving
};

USTRUCT(BlueprintType)
struct FCreepSpeeds
{
    GENERATED_BODY()

    float SpeedMoving = 600.0f;
    float SpeedRotating = 6.0f;
};

struct FCreepSpeedRandoms
{
    float MoveInterpSpeed = 1000.0f;
    float MoveInterpSpeedRand = 100.0f;
    float MovingRandom = 30.0f;
    float RotatingRandom = 2.0f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API UCreepMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCreepMovementComponent();
    virtual void BeginPlay() override;

    bool TickCreepMoving(float DeltaTime);
    bool TickCreepRotating(float DeltaTime, bool PostMoveRotation);
    void StopMoving();

    float GetCreepCurrentSpeedMoving() const { return CreepCurrentSpeeds.SpeedMoving; }
    float GetCreepCurrentSpeedRotating() const { return CreepCurrentSpeeds.SpeedRotating; }

    FVector& GetMovingDestination() { return MovingDestination; }
    void SetMovingDestination(FVector MovingDestinationIn);
    void SetCreepPostMovingRotation(FVector NewSquadForwardVectorIn);

protected:
    FCreepSpeeds CreepMaxSpeeds;

    FCreepSpeeds CreepCurrentSpeeds;

private:
    TObjectPtr<ABaseCreepActor> OwnerCreep;
    ECreepMovementState CreepMovementState = ECreepMovementState::idle;
    FVector MovingDestination = FVector::Zero();
    FVector NewSquadForwardVector = FVector::Zero();
    FQuat NewRotationQuat = FQuat::Identity;
    FCreepSpeedRandoms CreepSpeedRandoms;
    FTimerHandle TimerHandle;

    float DistSquaredEndMove = 50.0f;

    void UpdateMovingSpeed(float DeltaTime);
    void PostMovingRotation();
};
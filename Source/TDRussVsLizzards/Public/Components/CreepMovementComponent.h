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

    float SpeedMoving   = 500.0f;
    float SpeedRotating = 100.0f;
};

struct FCreepSpeedRandoms
{
    float MoveInterpSpeed       = 600.0f;
    float MoveInterpSpeedRand   = 50.0f;
    float MovingRandom          = 40.0f;
    float RotatingRandom        = 50.0f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API UCreepMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCreepMovementComponent();
    virtual void BeginPlay() override;

    bool TickCreepMoving(float& DeltaTime);
    bool TickCreepRotating(float& DeltaTime);

    float GetCreepCurrentSpeedMoving() { return CreepCurrentSpeeds.SpeedMoving; }
    float GetCreepCurrentSpeedRotating() { return CreepCurrentSpeeds.SpeedRotating; }

    FVector& GetMovingDestination() { return MovingDestination; }
    void SetMovingDestination(FVector MovingDestinationIn);

protected:
    FCreepSpeeds CreepMaxSpeeds;

    FCreepSpeeds CreepCurrentSpeeds;

private:
    ECreepMovementState CreepMovementState = ECreepMovementState::idle;
    FVector MovingDestination              = FVector::Zero();
    TObjectPtr<ABaseCreepActor> OwnerCreep;

    FCreepSpeedRandoms CreepSpeedRandoms;

    float DistSquaredEndMove       = 20.0f;
    float DistSquaredStopingMoving = 500.0f;

    void UpdateMovingSpeed(float& DeltaTime);
};

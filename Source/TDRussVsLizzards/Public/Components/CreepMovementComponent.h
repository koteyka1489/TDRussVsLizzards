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

    float SpeedMoving   = 100.0f;
    float SpeedRotating = 20.0f;
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

    void SetCreepMaxSpeeds(float SpeedRotatingIn, float SpeedMovingIn);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
    FCreepSpeeds CreepMaxSpeeds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
    FCreepSpeeds CreepCurrentSpeeds;

private:
    ECreepMovementState CreepMovementState = ECreepMovementState::idle;
    FVector MovingDestination              = FVector::Zero();
    TObjectPtr<ABaseCreepActor> OwnerCreep;

    float MoveInterpSpeed     = 20.0f;
    float MoveInterpSpeedRand = 5.0f;
    float RotateInterpSpeed     = 20.0f;
    float RotateInterpSpeedRand = 5.0f;


    float DistSquaredEndMove       = 20.0f;
    float DistSquaredStopingMoving = 250.0f;

    void UpdateMovingSpeed(float& DeltaTime);
};

// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CreepMovementComponent.generated.h"

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
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    float GetCreepCurrentSpeedMoving() { return CreepCurrentSpeeds.SpeedMoving; }
    float GetCreepCurrentSpeedRotating() { return CreepCurrentSpeeds.SpeedRotating; }

    FVector& GetMovingDestination() { return MovingDestination; }
    void SetMovingDestination(FVector MovingDestinationIn) { MovingDestination = MovingDestinationIn; }

    void SetCreepSpeeds(float SpeedRotatingIn, float SpeedMovingIn);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
    FCreepSpeeds CreepMaxSpeeds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
    FCreepSpeeds CreepCurrentSpeeds;

private:
    FVector MovingDestination = FVector::Zero();

};

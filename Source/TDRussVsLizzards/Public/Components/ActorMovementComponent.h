// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorMovementComponent.generated.h"

DECLARE_DELEGATE(FOnMovingComplete)

class ABaseCreepActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API UActorMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UActorMovementComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void MoveToLocation(FVector Location);

    FOnMovingComplete OnMovingComplete;

private:
    FVector DestinationToMoving{};
    FVector DestinationToRotating{};
    bool bDestinationToMovingIsSet   = false;
    bool bDestinationToRotatingIsSet = false;
    float SpeedMoving                = 500.0f;
    float SpeedRotating              = 300.0f;
    bool bAutoOrientToMovement       = true;

    void RotateToLocation(FVector Location);
    void MovingToLocation(float DeltaTime);
    void RotatingToLocation(float DeltaTime);
};

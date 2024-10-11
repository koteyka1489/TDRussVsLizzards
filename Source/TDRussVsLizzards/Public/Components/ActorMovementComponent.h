// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorMovementComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API UActorMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UActorMovementComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void MoveToLocation(FVector Location);
    void RotateToLocation(FVector Location);

    float SpeedMoving = 500.0f;
    float SpeedRotating = 150.0f;
    bool bAutoOrientToMovement = true;

private:
    FVector DestinationToMoving{};
    FVector DestinationToRotating{};
    bool bDestinationToMovingIsSet   = false;
    bool bDestinationToRotatingIsSet = false;

    void MovingToLocation(float DeltaTime);
    void RotatingToLocation(float DeltaTime);
};
